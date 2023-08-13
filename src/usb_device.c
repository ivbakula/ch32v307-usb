#include "usb_device.h"

#include <stdint.h>
#include <string.h>

#include "ch32v307.h"
#include "irq.h"
#include "time.h"
#include "usb-config/usb_device_full-speed.descr"
#include "usb_interface.h"

#define DEBUG

static struct USB_Device usb_device = {.state = 0,
  .max_packet_size_ctrl = 0,
  .max_packet_size_irq = 0,
  .ep0 = {
    .packet_size = 0,
    .tx_data_left = 0,
  }};

static void usb_write_ep0(void *data, size_t size)
{
  struct USB_Ctrl_Endpoint *ep0 = &usb_device.ep0;
  ep0->packet_size = (usb_device.max_packet_size_ctrl > size) ? size : usb_device.max_packet_size_ctrl;

  if (size) {
    ep0->tx_data_left = size;
    USBHD->UEP0_DMA = (uintptr_t)((uint8_t *)data);
  }
  USBHD->UEP0_TX_CTRL = (USBHD->UEP0_TX_CTRL & 0x08) ^ 0x08;
  USBHD->UEP0_TX_LEN = ep0->packet_size;
}

static void usb_update_ep0(void)
{
  struct USB_Ctrl_Endpoint *ep0 = &usb_device.ep0;
  ep0->tx_data_left -= ep0->packet_size;

  if (!ep0->tx_data_left) { /* No more data; Reset endpoint */
    USBHD->UEP0_TX_CTRL &= ~0x8;
    USBHD->UEP0_TX_LEN = 0;
    USBHD->UEP0_DMA = (uintptr_t)((uint8_t *)ep0->rx_dma_buffer);
    ep0->packet_size = 0;
    return;
  }

  USBHD->UEP0_DMA += ep0->packet_size;
  if (ep0->packet_size > ep0->tx_data_left)
    ep0->packet_size = ep0->tx_data_left;

  USBHD->UEP0_TX_CTRL = (USBHD->UEP0_TX_CTRL & 0x8) ^ 0x08;
  USBHD->UEP0_TX_LEN = ep0->packet_size;
}

/* ====================================================================== */
/* ===============================DEBUG================================== */
#ifdef DEBUG
// void gpio_port_config(uint32_t gpio_base, uint8_t port, uint8_t cfg);
static inline void enable_usbd_debug(void) __attribute__((always_inline));
static inline void enable_usbd_debug(void)
{
  /* RCC->R32_RCC_APB2PCENR |= RCC_APB2PCENR_IOPBEN; */
  /* gpio_port_config(GPIOB_BASE, 10, 0b0001); */
  /* gpio_port_config(GPIOB_BASE, 11, 0b0001); */
  /* gpio_port_config(GPIOB_BASE, 12, 0b0001); */
  /* gpio_port_config(GPIOB_BASE, 13, 0b0001); */
  /* gpio_port_config(GPIOB_BASE, 14, 0b0001); */
}

static uint32_t __dbg_pin;
/*
 * Logic analyzer config PINOUT:
 *
 * BOARD PIN | LOGIC ANALYZER CH | INTERRUPT
 * B12         CH2                 SETUP_IRQ
 * B13         CH3                 BUS_RST
 * B11         CH5                 SUSPEND
 * B10         CH4                 HST_SOF // NOT ANY MORE. This interrupt never occurs during initialization
 * B10         CH4                 TRANSFER_IRQ
 * B14         CH7                 ISO_ACT
 */
static inline void debug_interrupt_on(void) __attribute__((always_inline));
static inline void debug_interrupt_on(void)
{
  __dbg_pin = 0;
  if (USBHD->INT_FG & U8_BIT(0))
    __dbg_pin |= 1 << 13;

  if (USBHD->INT_FG & U8_BIT(2))
    __dbg_pin |= 1 << 11;

  /*
  if (USBHD->INT_FG & U8_BIT(3))
    __dbg_pin |= 1 << 10;
  */
  if (USBHD->INT_FG & U8_BIT(1))
    __dbg_pin |= 1 << 10;

  if (USBHD->INT_FG & U8_BIT(5))
    __dbg_pin |= 1 << 12;

  if (USBHD->INT_FG & U8_BIT(6))
    __dbg_pin |= 1 << 14;

  GPIOB->R32_GPIO_OUTDR |= __dbg_pin;
}

static inline void debug_interrupt_off(void) __attribute__((always_inline));
static inline void debug_interrupt_off(void)
{
  GPIOB->R32_GPIO_OUTDR &= ~__dbg_pin;
}
#endif
/* ====================================================================== */
/* ===============================DEBUG================================== */

static inline void handle_bus_rst(void) __attribute__((always_inline));
static inline void handle_bus_rst(void)
{
  USBHD->DEV_AD = 0;
  usb_device.address = 0;
}

static inline void do_handle_transfer_ep0(void) __attribute__((always_inline));
static inline void do_handle_transfer_ep0(void)
{
  switch (USBHD->INT_ST & TOKEN_MASK) {
    case TOKEN_IN:
      usb_update_ep0();
      if (usb_device.state == USBD_ADDRESS_ASSIGNEMENT)
        USBHD->DEV_AD = usb_device.address;
      break;

    case TOKEN_OUT:
      break;
  }
}

static inline void do_handle_transfer(void) __attribute__((always_inline));
static inline void do_handle_transfer(void)
{
  switch (USBHD->INT_ST & TOKEN_MASK) {
    case TOKEN_OUT:
      usb_device.ep1.rx_transfer_size = USBHD->RX_LEN;
      usb_device.ep1.rx_state = EP_RX_DATA_RDY;
      break;
    case TOKEN_IN:
      //      USBHD->UEP1_TX_CTRL = (USBHD->UEP0_TX_CTRL & 0x08) ^ 0x08;
      USBHD->UEP1_TX_LEN = 0;
      USBHD->UEP1_TX_CTRL |= 3;
      usb_device.ep1.tx_state = EP_IDLE;
      break;
    default:
      // unsupported
      break;
  }
}

static inline void handle_transfer(void) __attribute__((always_inline));
static inline void handle_transfer(void)
{
  uint8_t endpoint = USBHD->INT_ST & 0b111;
  switch (endpoint) {
    case 0:
      do_handle_transfer_ep0();
      break;
    case 1:
      do_handle_transfer();
      break;
    default:
      // not supported for now. We'll see later how to handle this
      break;
  }
}

static inline void handle_setup(void) __attribute__((always_inline));
static inline void handle_setup(void)
{
  SetupPacket *sp = (SetupPacket *)usb_device.ep0.rx_dma_buffer;
  struct USB_Ctrl_Endpoint *ep0 = &usb_device.ep0;
  void *data = 0;
  size_t size = 0;

  if (sp->bRequest == GET_DESCRIPTOR) {
    switch (sp->wValue >> 8) {
      case DEVICE_DESCRIPTOR:
        data = &deviceDescriptor;
        size = sizeof(DeviceDescriptor);
        break;

      case CONFIGURATION_DESCRIPTOR:
        if (usb_device.state == USBD_CONFIGURATION_EXCHANGE) {
          data = &compoundDescriptor;
          size = sizeof(compoundDescriptor);
        } else {
          data = &compoundDescriptor.configurationDescriptor;
          size = sizeof(compoundDescriptor.configurationDescriptor);
          usb_device.state = USBD_CONFIGURATION_EXCHANGE;
        }
        break;

      case STRING_DESCRIPTOR:
        if (sp->wIndex == 0) {
          data = &stringDescriptorLang;
          size = sizeof(struct StringDescriptorLang);
        } else {
          data = &stringDescriptorStr;
          size = sizeof(struct StringDescriptorStr);
        }

      default:
        // do nothing.
        break;
    }
  } else if (sp->bRequest == SET_ADDRESS) {
    usb_device.state = USBD_ADDRESS_ASSIGNEMENT;
    usb_device.address = sp->wValue;
  } else if (sp->bRequest == SET_CONFIGURATION) {
    // THIS MEANS THAT ENUMERATION PROCESS IS DONE SO JUST CHANGE STATE TO READY
    usb_device.state = USBD_READY;
  }

  // Every SETUP transaction ends with IN token, so we need to write something
  // to the EP0.
  usb_write_ep0(data, size);
}

static void usb_irqhandler(void)
{
#ifdef DEBUG
  debug_interrupt_on();
#endif
  switch (USBHD->INT_FG) {
    case USBHD_INTFG_BUSRST:
      handle_bus_rst();
      break;

    case USBHD_INTFG_TRANS:
      handle_transfer();
      break;

    case USBHD_INTFG_SUSPEND:
      break;

    case USBHD_INTFG_FIFO:
      break;

    case USBHD_INTFG_SETUP:
      handle_setup();
      break;
  }

#ifdef DEBUG
  debug_interrupt_off();
#endif

  /* Clear interrupt flag */
  USBHD->INT_FG = USBHD->INT_FG;
}

static inline void usb_hw_init(USB_DeviceMode mode) __attribute__((always_inline));
static inline void usb_hw_init(USB_DeviceMode mode)
{
  /* RCC clock enable and clock initialize */
  RCC->R32_RCC_CFGR2 |= USB_HSSRC_USBPHY | USB_HSCLK_4MHz | USB_HSPLL_EN;
  RCC->R32_RCC_AHBPCENR |= RCC_AHBPCENR_USBHSEN;

  /* Reset USB processor */
  USBHD->CONTROL = USBHD_CONTROL_RESET_SIE | USBHD_CONTROL_CLR_ALL;
  wait_us(10);

  USBHD->CONTROL &= ~USBHD_CONTROL_RESET_SIE;      // CLEAR RESET BIT
  USBHD->HOST_CTRL = USBHD_HOST_CTRL_PHY_SUSPEND;  // I have no idea why is this necessary

  /* Enable some interrupts */
  USBHD->INT_EN = USBHD_INTEN_BUSRST | USBHD_INTEN_TRANSFER | USBHD_INTEN_SUSPEND | USBHD_INTEN_SOFACT | USBHD_INTEN_FIFO |
                  USBHD_INTEN_SETUP;

  USBHD->CONTROL = USBHD_CONTROL_DMA_EN | USBHD_CONTROL_BSY | USBHD_CONTROL_SPEED_TYPE(mode);

  usb_device.max_packet_size_irq = usb_device.max_packet_size_ctrl = USB_MAX_PACKET_SIZE;  //(mode == USB_LOW_SPEED) ? 8 : 64;

  /* Initialize endpoint 0 */
  USBHD->UEP0_MAX_LEN = usb_device.max_packet_size_ctrl;
  USBHD->UEP0_DMA = (uintptr_t)((uint8_t *)usb_device.ep0.rx_dma_buffer);

  USBHD->UEP0_TX_LEN = 0;
  USBHD->UEP0_TX_CTRL = USBHD_TXCTRL_RES_NAK; /* Respond NAK to IN packets on EP0 */
  USBHD->UEP0_TX_LEN = 0;                     /* Nothing to transmit yet */
  USBHD->UEP0_RX_CTRL = 0x00;                 /* Data ready and expect ACK */

  /* Initialize endpoint 1 */
  USBHD->ENDP_CONFIG = U32_BIT(1) | U32_BIT(17); /* Enable EP1 Tx and EP1 Rx */
  USBHD->UEP1_MAX_LEN = usb_device.max_packet_size_irq;

  USBHD->UEP1_TX_DMA = (uintptr_t)((uint8_t *)usb_device.ep1.tx_dma_buffer);
  USBHD->UEP1_RX_DMA = (uintptr_t)((uint8_t *)usb_device.ep1.rx_dma_buffer);

  USBHD->UEP1_RX_CTRL = 0;
  USBHD->UEP1_TX_CTRL = U8_BIT(5) | U8_BIT(1);

  /* Register USB device interrupt handler and enable USB device interrupt line */
  irq_register_interrupt_handler(USBHS_IRQn, usb_irqhandler);
  irq_enable_interrupt(USBHS_IRQn);

  /* Enable internal pull-up resistor */
  USBHD->CONTROL |= USBHD_CONTROL_PUEN;
}

void init_usb_device(USB_DeviceMode mode)
{
  usb_hw_init(mode);
#ifdef DEBUG
  enable_usbd_debug();
#endif
}

/* TODO make this function more generic (Read every possible endpoint). */
size_t read_endpoint(char *buffer, size_t buffer_sz, int endpoint)
{
  while (usb_device.ep1.rx_state != EP_RX_DATA_RDY)
    ;

  size_t cpy_sz = (usb_device.ep1.rx_transfer_size > buffer_sz) ? buffer_sz : usb_device.ep1.rx_transfer_size;

  memcpy(buffer, usb_device.ep1.rx_dma_buffer, cpy_sz);

  usb_device.ep1.rx_state = EP_IDLE;
  return cpy_sz;
}

void write_endpoint(char *buffer, size_t buffer_sz, int endpoint)
{
  /* while (usb_device.ep1.tx_state != EP_IDLE) */
  /*   ; */

  size_t cpy_sz = (buffer_sz > 64) ? 64 : buffer_sz;

  memcpy(usb_device.ep1.tx_dma_buffer, buffer, cpy_sz);
  USBHD->UEP1_TX_LEN = cpy_sz;
  USBHD->UEP1_TX_CTRL &= ~0b11;
  usb_device.ep1.tx_state = EP_TX_PROGRESS;
}
