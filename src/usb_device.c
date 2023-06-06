#include "usb_device.h"
#include <stdint.h>
#include "ch32v307.h"
#include "irq.h"
#include "time.h"
#include "common.h"
#include "usb_device.descr"
#include "usb_interface.h"

static struct USB_Device usb_device = {
  .state                = USBD_UNINITIALIZED,
  .max_packet_size_ctrl = 0,
  .max_packet_size_irq  = 0,
  .ep0 = {
    .packet_size = 0,
    .tx_data_left = 0,
    .status = EP_IDLE
  }
};

/* TODO generalize this function */
static void write_endpoint0(void *data, size_t size)
{
  struct USB_Ctrl_Endpoint *ep0 = &usb_device.ep0;
  USBHD->UEP0_DMA               = (uintptr_t)((uint8_t *)data);

  ep0->tx_data_left = size;
  ep0->packet_size = (size < usb_device.max_packet_size_ctrl) ? size : usb_device.max_packet_size_ctrl;

  USBHD->UEP0_TX_CTRL = (USBHD->UEP0_TX_CTRL & 0x8) ^ 0x8;
  USBHD->UEP0_TX_LEN = ep0->packet_size;
}

static void reset_enpoint0(void)
{
  USBHD->UEP0_DMA = (uintptr_t)((uint8_t *)usb_device.ep0.rx_dma_buffer);
  USBHD->UEP0_TX_LEN = 0;
  USBHD->UEP0_TX_CTRL = USBHD_TXCTRL_RES_NAK; /* Respond NAK to IN packets on EP0 */      
}

/* ====================================================================== */
/* ===============================DEBUG================================== */
#ifdef DEBUG
void gpio_port_config(uint32_t gpio_base, uint8_t port, uint8_t cfg);
static inline void enable_usbd_debug(void) __attribute__((always_inline));
static inline void enable_usbd_debug(void)
{
  RCC->R32_RCC_APB2PCENR |= RCC_APB2PCENR_IOPBEN;
  gpio_port_config(GPIOB_BASE, 10, 0b0001);
  gpio_port_config(GPIOB_BASE, 11, 0b0001);  
  gpio_port_config(GPIOB_BASE, 12, 0b0001);
  gpio_port_config(GPIOB_BASE, 13, 0b0001);
  gpio_port_config(GPIOB_BASE, 14, 0b0001);
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

  if (USBHD->INT_FG & U8_BIT(1))
    __dbg_pin |= 1 << 10;

  if (USBHD->INT_FG & U8_BIT(5))
    __dbg_pin |= 1 << 12;

  if (USBHD->INT_FG & U8_BIT(6))
    __dbg_pin |= 1 << 14;

  GPIOB->R32_GPIO_OUTDR |= pin;
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
  // TODO handle reset event properly
}

static inline void handle_transfer(void) __attribute__((always_inline));
static inline void handle_transfer(void)
{
  // TODO handle transmission complete event
}

static inline void handle_suspend(void) __attribute__((always_inline));
static inline void handle_suspend(void)
{
  // TODO handle suspend event
}

static inline void handle_fifo_overflow(void) __attribute__((always_inline));
static inline void handle_fifo_overflow(void)
{
  // TODO do something about fifo overflow.
}

static inline void handle_setup(void) __attribute__((always_inline));
static inline void handle_setup(void)
{
  SetupPacket *sp = (SetupPacket *)usb_device.ep0.rx_dma_buffer;
  struct USB_Ctrl_Endpoint *ep0 = &usb_device.ep0;
  
  if (sp->bRequest == GET_DESCRIPTOR) {
    switch (sp->wValue >> 8)
    {
      case DEVICE_DESCRIPTOR:
        write_endpoint0(&deviceDescriptor, sizeof(DeviceDescriptor));
        break;

      case CONFIGURATION_DESCRIPTOR:
        write_endpoint0(&compoundDescriptor.configurationDescriptor, sizeof(ConfigurationDescriptor));
        break;

      case STRING_DESCRIPTOR:
        
      default:
        // do nothing.
        break;
    }
  } else if (sp->bRequest == SET_ADDRESS) {
    
  } else {
    // do nothing. We're not handling this case.
  }
}

static void usb_irqhandler(void)
{
#ifdef DEBUG
  debug_interrupt_on();
#endif
  switch (USBHD->INT_FG)
  {
    case USBHD_INTFG_BUSRST:
    case USBHD_INTFG_TRANS:
    case USBHD_INTFG_SUSPEND:
    case USBHD_INTFG_FIFO:
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

  USBHD->CONTROL &= ~USBHD_CONTROL_RESET_SIE; // CLEAR RESET BIT
  USBHD->HOST_CTRL = USBHD_HOST_CTRL_PHY_SUSPEND; // I have no idea why is this necessary

  /* Enable some interrupts */
  USBHD->INT_EN = USBHD_INTEN_BUSRST | USBHD_INTEN_TRANSFER | USBHD_INTEN_SUSPEND |
                  USBHD_INTEN_FIFO | USBHD_INTEN_SETUP;

  USBHD->CONTROL = USBHD_CONTROL_DMA_EN | USBHD_CONTROL_BSY | USBHD_CONTROL_SPEED_TYPE(mode);

  usb_device.max_packet_size_irq = usb_device.max_packet_size_ctrl = (mode == USB_LOW_SPEED) ? 8 : 64;

  /* Initialize endpoint 0 */
  USBHD->UEP0_MAX_LEN = usb_device.max_packet_size_ctrl;
  USBHD->UEP0_DMA     = (uintptr_t)((uint8_t *)usb_device.ep0.rx_dma_buffer);

  USBHD->UEP0_TX_LEN = 0;
  USBHD->UEP0_TX_CTRL = USBHD_TXCTRL_RES_NAK; /* Respond NAK to IN packets on EP0 */
  USBHD->UEP0_TX_LEN  = 0;                    /* Nothing to transmit yet */
  USBHD->UEP0_RX_CTRL = 0x00;                 /* Data ready and expect ACK */

  /* Register USB device interrupt handler and enable USB device interrupt line */
  irq_register_interrupt_handler(USBHS_IRQn, usb_irqhandler);
  irq_enable_interrupt(USBHS_IRQn);

  /* Enable internal pull-up resistor */
  USBHD->CONTROL |=USBHD_CONTROL_PUEN;
}

void init_usb_device(USB_DeviceMode mode)
{
  usb_device.state = USBD_UNINITIALIZED;
  usb_hw_init(mode);

  #ifdef DEBUG
  enable_usbd_debug();
  #endif
  usb_device.state = USBD_INITIALIZED;
}    
