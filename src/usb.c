#include "ch32v307.h"
#include "time.h"
#include "irq.h"
#include <stdio.h>
#include <string.h>
#include "usb.h"
#include "msgq.h"

typedef struct {
  /* STOLEN from ch32v307 repo */
  volatile uint8_t  CONTROL;
  volatile uint8_t  HOST_CTRL;
  volatile uint8_t  INT_EN;
  volatile uint8_t  DEV_AD;
  volatile uint16_t FRAME_NO;
  volatile uint8_t  SUSPEND;
  volatile uint8_t  RESERVED0;
  volatile uint8_t  SPEED_TYPE;
  volatile uint8_t  MIS_ST;
  volatile uint8_t  INT_FG;
  volatile uint8_t  INT_ST;
  volatile uint16_t RX_LEN;
  volatile uint16_t RESERVED1;
  volatile uint32_t ENDP_CONFIG;
  volatile uint32_t ENDP_TYPE;
  volatile uint32_t BUF_MODE;
  volatile uint32_t UEP0_DMA;               
  volatile uint32_t UEP1_RX_DMA;       
  volatile uint32_t UEP2_RX_DMA;       
  volatile uint32_t UEP3_RX_DMA;       
  volatile uint32_t UEP4_RX_DMA;       
  volatile uint32_t UEP5_RX_DMA;       
  volatile uint32_t UEP6_RX_DMA;       
  volatile uint32_t UEP7_RX_DMA;       
  volatile uint32_t UEP8_RX_DMA;      
  volatile uint32_t UEP9_RX_DMA;       
  volatile uint32_t UEP10_RX_DMA;      
  volatile uint32_t UEP11_RX_DMA;      
  volatile uint32_t UEP12_RX_DMA;      
  volatile uint32_t UEP13_RX_DMA;      
  volatile uint32_t UEP14_RX_DMA;      
  volatile uint32_t UEP15_RX_DMA;      
  volatile uint32_t UEP1_TX_DMA;       
  volatile uint32_t UEP2_TX_DMA;       
  volatile uint32_t UEP3_TX_DMA;       
  volatile uint32_t UEP4_TX_DMA;       
  volatile uint32_t UEP5_TX_DMA;      
  volatile uint32_t UEP6_TX_DMA;      
  volatile uint32_t UEP7_TX_DMA;       
  volatile uint32_t UEP8_TX_DMA;      
  volatile uint32_t UEP9_TX_DMA;       
  volatile uint32_t UEP10_TX_DMA;      
  volatile uint32_t UEP11_TX_DMA;      
  volatile uint32_t UEP12_TX_DMA;    
  volatile uint32_t UEP13_TX_DMA;      
  volatile uint32_t UEP14_TX_DMA;     
  volatile uint32_t UEP15_TX_DMA;      
  volatile uint16_t UEP0_MAX_LEN;
  volatile uint16_t RESERVED2;
  volatile uint16_t UEP1_MAX_LEN;
  volatile uint16_t RESERVED3;
  volatile uint16_t UEP2_MAX_LEN;
  volatile uint16_t RESERVED4;
  volatile uint16_t UEP3_MAX_LEN;
  volatile uint16_t RESERVED5;
  volatile uint16_t UEP4_MAX_LEN;
  volatile uint16_t RESERVED6;
  volatile uint16_t UEP5_MAX_LEN;
  volatile uint16_t RESERVED7;
  volatile uint16_t UEP6_MAX_LEN;
  volatile uint16_t RESERVED8;
  volatile uint16_t UEP7_MAX_LEN;
  volatile uint16_t RESERVED9;
  volatile uint16_t UEP8_MAX_LEN;
  volatile uint16_t RESERVED10;
  volatile uint16_t UEP9_MAX_LEN;
  volatile uint16_t RESERVED11;
  volatile uint16_t UEP10_MAX_LEN;
  volatile uint16_t RESERVED12;
  volatile uint16_t UEP11_MAX_LEN;
  volatile uint16_t RESERVED13;
  volatile uint16_t UEP12_MAX_LEN;
  volatile uint16_t RESERVED14;
  volatile uint16_t UEP13_MAX_LEN;
  volatile uint16_t RESERVED15;
  volatile uint16_t UEP14_MAX_LEN;
  volatile uint16_t RESERVED16;
  volatile uint16_t UEP15_MAX_LEN;
  volatile uint16_t RESERVED17;
  volatile uint16_t UEP0_TX_LEN;
  volatile uint8_t  UEP0_TX_CTRL;
  volatile uint8_t  UEP0_RX_CTRL;
  volatile uint16_t UEP1_TX_LEN;
  volatile uint8_t  UEP1_TX_CTRL;
  volatile uint8_t  UEP1_RX_CTRL;
  volatile uint16_t UEP2_TX_LEN;
  volatile uint8_t  UEP2_TX_CTRL;
  volatile uint8_t  UEP2_RX_CTRL;
  volatile uint16_t UEP3_TX_LEN;
  volatile uint8_t  UEP3_TX_CTRL;
  volatile uint8_t  UEP3_RX_CTRL;
  volatile uint16_t UEP4_TX_LEN;
  volatile uint8_t  UEP4_TX_CTRL;
  volatile uint8_t  UEP4_RX_CTRL;
  volatile uint16_t UEP5_TX_LEN;
  volatile uint8_t  UEP5_TX_CTRL;
  volatile uint8_t  UEP5_RX_CTRL;
  volatile uint16_t UEP6_TX_LEN;
  volatile uint8_t  UEP6_TX_CTRL;
  volatile uint8_t  UEP6_RX_CTRL;
  volatile uint16_t UEP7_TX_LEN;
  volatile uint8_t  UEP7_TX_CTRL;
  volatile uint8_t  UEP7_RX_CTRL;
  volatile uint16_t UEP8_TX_LEN;
  volatile uint8_t  UEP8_TX_CTRL;
  volatile uint8_t  UEP8_RX_CTRL;
  volatile uint16_t UEP9_TX_LEN;
  volatile uint8_t  UEP9_TX_CTRL;
  volatile uint8_t  UEP9_RX_CTRL;
  volatile uint16_t UEP10_TX_LEN;
  volatile uint8_t  UEP10_TX_CTRL;
  volatile uint8_t  UEP10_RX_CTRL;
  volatile uint16_t UEP11_TX_LEN;
  volatile uint8_t  UEP11_TX_CTRL;
  volatile uint8_t  UEP11_RX_CTRL;
  volatile uint16_t UEP12_TX_LEN;
  volatile uint8_t  UEP12_TX_CTRL;
  volatile uint8_t  UEP12_RX_CTRL;
  volatile uint16_t UEP13_TX_LEN;
  volatile uint8_t  UEP13_TX_CTRL;
  volatile uint8_t  UEP13_RX_CTRL;
  volatile uint16_t UEP14_TX_LEN;
  volatile uint8_t  UEP14_TX_CTRL;
  volatile uint8_t  UEP14_RX_CTRL;
  volatile uint16_t UEP15_TX_LEN;
  volatile uint8_t  UEP15_TX_CTRL;
  volatile uint8_t  UEP15_RX_CTRL;  
} USBHD_Regfile;


#define USBHD_BASE ((uint32_t)0x40023400)
#define USBHD ((USBHD_Regfile *) USBHD_BASE)
#define USBHD_CONTROL_RESET 0x06

#define USBHD_CONTROL_DMA_EN U32_BIT(0)
#define USBHD_CONTROL_DEV_PU_EN U8_BIT(4) /* Enable USB device and pull-up resistor */
#define USBHD_INTEN_SETUP_ACT U8_BIT(5)
#define USBHD_REQUIRED_CLOCK_FREQ 48000000

#define USBHD_INTFG_SETUP_ACT U8_BIT(5) 

#define DEF_USBD_UEP0_SIZE           64     /* usb hs/fs device end-point 0 size */

__attribute__((aligned(4))) uint8_t USBHS_EP0_Buf[DEF_USBD_UEP0_SIZE];
__attribute__((aligned(4))) uint8_t USBHS_EP3TX_Buf[DEF_USBD_UEP0_SIZE];
__attribute__((aligned(4))) uint8_t USBHS_EP3RX_Buf[DEF_USBD_UEP0_SIZE];

#define DEF_USB_VID                  0x1A86
#define DEF_USB_PID 0xFE0C
#define DEF_FILE_VERSION             0x01
#define DEF_IC_PRG_VER DEF_FILE_VERSION

const uint8_t  MyDevDescr[ ] =
{
    0x12,       // bLength
    0x01,       // bDescriptorType (Device)
    0x00, 0x02, // bcdUSB 2.00
    0x02,       // bDeviceClass
    0x00,       // bDeviceSubClass
    0x00,       // bDeviceProtocol
    DEF_USBD_UEP0_SIZE,   // bMaxPacketSize0 64
    (uint8_t)DEF_USB_VID, (uint8_t)(DEF_USB_VID >> 8),  // idVendor 0x1A86
    (uint8_t)DEF_USB_PID, (uint8_t)(DEF_USB_PID >> 8),  // idProduct 0x5537
    DEF_IC_PRG_VER, 0x00, // bcdDevice 0.01
    0x01,       // iManufacturer (String Index)
    0x02,       // iProduct (String Index)
    0x03,       // iSerialNumber (String Index)
    0x01,       // bNumConfigurations 1
};

#define USB_EP0_SP ((SetupPacket *)USBHS_EP0_Buf)

static MSG_queue ep0_msgq_tx;
static MSG_queue ep0_msgq_rx;

size_t usb_setup_read_ep0(void *buffer)
{
  return msgq_dequeue(&ep0_msgq_rx, buffer, true);
}

size_t usb_setup_write_ep0(void *buffer, int size_left)
{
  //  int size_left = 0x12;
  //  int msg_size = 8;
  //size_t ret   = 0;
  return msgq_enqueue(&ep0_msgq_tx, buffer, size_left, true);

  /* while (size_left > 0) */
  /* { */
  /*   if (size_left < 8) */
  /*     msg_size = size_left; */

  /*   msgq_enqueue(&ep0_msgq_tx, buffer, size_left, true); */
    
  /*   /\* while ((USBHD->INT_FG & 3 << 4) != TOKEN_IN) *\/ */
  /*   /\*   wait_us(64); *\/ */

  /*   /\* char fmt[32] = { 0 }; *\/ */
  /*   /\* sprintf(fmt, "msg_size: %d\r\n", msg_size); *\/ */
  /*   /\* uart_puts(fmt); *\/ */
    
  /*   /\* memcpy(USBHS_EP0_Buf, buffer, msg_size); *\/ */
  /*   /\* USBHD->UEP0_TX_CTRL = (USBHD->UEP0_TX_CTRL & 0x8) ^ 0x8; *\/ */
  /*   /\* USBHD->UEP0_TX_LEN  = msg_size; *\/ */
  /*   /\* buffer = (void *)((uintptr_t) buffer + msg_size); *\/ */
  /*   /\* wait_us(100); *\/ */

  /*   size_left -= msg_size; */
  /*   ret += msg_size; */
  /* } */

  //  USBHD->UEP0_TX_LEN = 0;
  //  return ret;
}

void usb_assign_address(uint8_t address)
{
  USBHD->DEV_AD = address;
}

DeviceDescriptor dev_descr = { .bLength            = sizeof(dev_descr),
                               .bDescriptorType    = 0x01,
                               .bcdUSB             = 0x0001,
                               .bDeviceClass       = 0x02,
                               .bDeviceSubClass    = 0x00,
                               .bDeviceProtocol    = 0x00,
                               .bMaxPacketSize0    = 8,
                               .idVendor           = 0x6976,
                               .idProduct          = 0x616e,
                               .bcdDevice          = 0x01,
                               .iProduct           = 0x01,
                               .iSerialNumber      = 0x00,
                               .bNumConfigurations = 0x1 };

CompoundDescriptor cdscr = {
  .config_descr = {
    .bLength         = sizeof(ConfigurationDescriptor),
    .bDescriptorType = CONFIGURATION_DESCRIPTOR,
    .wTotalLength =
    sizeof(ConfigurationDescriptor) +
    sizeof(InterfaceDescriptor) * 2 +
    sizeof(EndpointDescriptor) * 2,
    .bNumInterfaces      = 0x02,
    .bConfigurationValue = 0x01,
    .iConfiguration      = 0x00,
    .bmAttributes        = U8_BIT(7),
    .bMaxPower           = 100,
  },

  .iface_descr = {
    .bLength            = sizeof(InterfaceDescriptor),
    .bDescriptorType    = INTERFACE_DESCRIPTOR,
    .bInterfaceNumber   = 0x0,
    .bAlternateSetting  = 0x0,
    .bNumEndpoints      = 0x01,
    .bInterfaceClass    = 0x03,
    .bInterfaceSubClass = 0x00,
    .bInterfaceProtocol = 0x00,
    .iInterface         = 0x00,
  },

  .ep_descr = {
    .bLength          = sizeof(EndpointDescriptor),
    .bDescriptorType  = ENDPOINT_DESCRIPTOR,
    .bEndpointAddress = 3 | U8_BIT(7),
    .bmAttributes     = 0x3,
    .wMaxPacketSize   = 0x8,
    .bInterval        = 0x10,
  },

  .iface_descr2 = {
    .bLength            = sizeof(InterfaceDescriptor),
    .bDescriptorType    = INTERFACE_DESCRIPTOR,
    .bInterfaceNumber   = 0x1,
    .bAlternateSetting  = 0x0,
    .bNumEndpoints      = 0x01,
    .bInterfaceClass    = 0x03,
    .bInterfaceSubClass = 0x00,
    .bInterfaceProtocol = 0x00,
    .iInterface         = 0x00,
  },

  .ep_descr2 = {
    .bLength = sizeof(EndpointDescriptor),
    .bDescriptorType = ENDPOINT_DESCRIPTOR,
    .bEndpointAddress = 3,
    .bmAttributes = 0x3,
    .wMaxPacketSize = 0x8,
    .bInterval = 0x10,
  }
};

StringDescriptor str_descr = { .bLength         = sizeof(StringDescriptor),
                               .bDescriptorType = STRING_DESCRIPTOR,
                               .wLANGID[0]      = 0x0409 };

USB_DescriptorString usb_str_descr = {
  .bLength         = sizeof(usb_str_descr) + 22,
  .bDescriptorType = STRING_DESCRIPTOR,
  //  .bString         = { 'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd' }
  .bString = {
    'H', 0x00,
    'e', 0x00,
    'l', 0x00,
    'l', 0x00,
    'o', 0x00,
    ' ', 0x00,
    'W', 0x00,
    'o', 0x00,
    'r', 0x00,
    'l', 0x00,
    'd', 0x00},
};

uint8_t address = 0;
uint8_t address_assignement = 0;

uint8_t get_configuration = 0;
volatile uint8_t is_configured = 0;
char set_config_cfm[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static void handle_setup_packet(void)
{
  SetupPacket *sp = (SetupPacket *)(USBHS_EP0_Buf);
  switch (sp->bRequest)
  {
    case GET_DESCRIPTOR:
      switch (sp->wValue >> 8)
      {
        case DEVICE_DESCRIPTOR:
          msgq_enqueue(&ep0_msgq_tx, &dev_descr, sizeof(dev_descr), false);
          break;

        case CONFIGURATION_DESCRIPTOR:
          if (!get_configuration)
          {
            msgq_enqueue(&ep0_msgq_tx, &cdscr.config_descr, sizeof(ConfigurationDescriptor), false);
            get_configuration = 1;
          }
          else
          {
            msgq_enqueue(&ep0_msgq_tx, &cdscr, cdscr.config_descr.wTotalLength, false);
          }
          break;

        case STRING_DESCRIPTOR:
          if (sp->wIndex == 0)
            msgq_enqueue(&ep0_msgq_tx, &str_descr, sizeof(str_descr), false);
          else
          {
            msgq_enqueue(&ep0_msgq_tx, &usb_str_descr, usb_str_descr.bLength, false);
          }
          break;

        case INTERFACE_DESCRIPTOR:
            break;
          
        case ENDPOINT_DESCRIPTOR:
            break;
      }

      break;
    case SET_ADDRESS:
      address = sp->wValue;
      address_assignement = 1;
      break;

    case SET_CONFIGURATION:
      //      msgq_enqueue(&ep0_msgq_tx, (void *)set_config_cfm, 8, false);
      USBHD->UEP0_TX_CTRL &= ~0x8;
      is_configured = 1;
      break;
  }
}

volatile int out_transaction_in_progress = 0;
int out_transaction_size        = 11;
volatile int data_rdy = 0;
MSG_queue ep3_rx_msgq;
void handle_token_out()
{
  int size = 8;

  if ((USBHD->INT_ST & 15) != 3)
  {
    return;
  }

  char buf[256] = { 0 };  
  if (!out_transaction_in_progress)
  {
    //    out_transaction_size = ((USB_DataPacket *)USBHS_EP0_Buf)->_size;
    //    sprintf(buf, "out_transaction_size: %d\r\n", out_transaction_size);
    out_transaction_in_progress = 1;
  }

  if (out_transaction_size < size)
    size = out_transaction_size;

  sprintf(buf, "data recvd: %s\r\n", USBHS_EP3RX_Buf);
  msgq_enqueue(&ep3_rx_msgq, USBHS_EP3RX_Buf, size, false);
  out_transaction_size -= size;

  if (out_transaction_size == 0)
  {
    out_transaction_in_progress = 0;
    data_rdy = 1;
  }    

}

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
static void usbhd_irq(void)
{
  uint32_t pin = 0;

  if (USBHD->INT_FG & U8_BIT(0))
    pin |= 1 << 13;

  if (USBHD->INT_FG & U8_BIT(2))
    pin |= 1 << 11;

  if (USBHD->INT_FG & U8_BIT(1))
    pin |= 1 << 10;

  if (USBHD->INT_FG & U8_BIT(5))
    pin |= 1 << 12;

  if (USBHD->INT_FG & U8_BIT(6))
    pin |= 1 << 14;

  GPIOB->R32_GPIO_OUTDR |= pin;

  if (USBHD->INT_FG & U8_BIT(0))
  {
    USBHD->DEV_AD = 0;
  }

  if (USBHD->INT_FG & U8_BIT(5))
  {
    handle_setup_packet();
    int sz = msgq_dequeue(&ep0_msgq_tx, USBHS_EP0_Buf, false);
    USBHD->UEP0_TX_CTRL = (USBHD->UEP0_TX_CTRL & 0x8) ^ 0x8;
    USBHD->UEP0_TX_LEN  = sz;
  }

  if (USBHD->INT_FG & U8_BIT(1))
  {
    if ((USBHD->INT_ST & TOKEN_MASK) == TOKEN_IN)
    {
      int sz = 0;
      if (!address_assignement)
      {
        sz = msgq_dequeue(&ep0_msgq_tx, USBHS_EP0_Buf, false);
      }
      else
      {
        USBHD->DEV_AD = address;
        address_assignement = 0;
      }
      USBHD->UEP0_TX_CTRL = (USBHD->UEP0_TX_CTRL & 0x8) ^ 0x8;
      USBHD->UEP0_TX_LEN  = sz;
    }
    if ((USBHD->INT_ST & TOKEN_MASK) == TOKEN_OUT)
    {
      if ((USBHD->INT_ST & 3) == 3)
      {
        uart_puts("EP3\r\n");
        handle_token_out();
      }

    }
  }

  USBHD->INT_FG = USBHD->INT_FG;
  GPIOB->R32_GPIO_OUTDR &= ~pin;  
}

void gpio_port_config(uint32_t gpio_base, uint8_t port, uint8_t cfg);    
void enable_usbd_debug(void)
{
  RCC->R32_RCC_APB2PCENR |= RCC_APB2PCENR_IOPBEN;
  gpio_port_config(GPIOB_BASE, 10, 0b0001);
  gpio_port_config(GPIOB_BASE, 11, 0b0001);  
  gpio_port_config(GPIOB_BASE, 12, 0b0001);
  gpio_port_config(GPIOB_BASE, 13, 0b0001);
  gpio_port_config(GPIOB_BASE, 14, 0b0001);  
}

size_t usb_poll(void *buffer, uint8_t endpoint, size_t buffer_size)
{
  size_t size = 0;
  while (!data_rdy)
    ;

  memset(buffer, 0, 64);
  uart_puts("DataRDY\r\n");
  size = msgq_dequeue(&ep3_rx_msgq, buffer, false);
  char str[256] = {};
  sprintf(str, "DATA_RECVD: %s\r\n", (char *)buffer);
  uart_puts(str);
  data_rdy = 0;
  
  return size;
}

void enable_usbd(void)
{
  if (sysclock_frequency != USBHD_REQUIRED_CLOCK_FREQ)
  {
    // If sysclock (PLL) is not 48MHz use USB PHY internal PLL as clock. Configure it here
  }
  RCC->R32_RCC_CFGR2 |= U32_BIT(31) | U32_BIT(29);
  RCC->R32_RCC_CFGR2 |= U32_BIT(30);
  RCC->R32_RCC_AHBPCENR |= RCC_AHBPCENR_USBHSEN;

  USBHD->CONTROL = 0x2 | 0x4; //USBHD_CONTROL_RESET;
  wait_us(10);
  USBHD->CONTROL &= ~0x4; // ~U8_BIT(2);
  USBHD->HOST_CTRL = 0x10;
  USBHD->CONTROL = USBHD_CONTROL_DMA_EN | U8_BIT(3) | U8_BIT(6);

  //  USBHD->INT_EN = 255;
  USBHD->INT_EN = U8_BIT(0) | U8_BIT(1) | U8_BIT(2) | U8_BIT(4) | U8_BIT(5) | U8_BIT(6);
  USBHD->UEP0_MAX_LEN = DEF_USBD_UEP0_SIZE;
  USBHD->UEP0_DMA = (uintptr_t)((uint8_t *)USBHS_EP0_Buf);

  USBHD->ENDP_CONFIG = 8 | 0x00080000;
  USBHD->UEP3_MAX_LEN = 8;
  
  USBHD->UEP3_TX_DMA  = (uintptr_t)((uint8_t *)USBHS_EP3TX_Buf);
  USBHD->UEP3_RX_DMA  = (uintptr_t)((uint8_t *)USBHS_EP3RX_Buf);

  USBHD->UEP3_RX_CTRL = 0;
  USBHD->UEP3_TX_CTRL = 2;
  USBHD->UEP0_TX_LEN = 0;
  USBHD->UEP0_TX_CTRL = 0x02;
  USBHD->UEP0_RX_CTRL = 0x00;

  msgq_init(&ep0_msgq_rx);
  msgq_init(&ep0_msgq_tx);
  msgq_init(&ep3_rx_msgq);
  irq_register_interrupt_handler(USBHS_IRQn, usbhd_irq);
  enable_usbd_debug();
  irq_enable_interrupt(USBHS_IRQn);
  USBHD->CONTROL |= USBHD_CONTROL_DEV_PU_EN;  
}    
