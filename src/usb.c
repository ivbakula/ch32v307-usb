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
extern void uart_puts(const char *str);
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
  
  if (USBHD->INT_FG & U8_BIT(5))
    msgq_enqueue(&ep0_msgq_rx, USBHS_EP0_Buf, 64, false);

  if (USBHD->INT_FG & U8_BIT(6))
  {
    int sz = msgq_dequeue(&ep0_msgq_tx, USBHS_EP0_Buf, false);
    USBHD->UEP0_TX_CTRL = (USBHD->UEP0_TX_CTRL & 0x8) ^ 0x8;
    USBHD->UEP0_TX_LEN  = sz;
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
  USBHD->UEP0_TX_LEN = 0;
  USBHD->UEP0_TX_CTRL = 0x02;
  USBHD->UEP0_RX_CTRL = 0x00;

  msgq_init(&ep0_msgq_rx);
  msgq_init(&ep0_msgq_tx);
  irq_register_interrupt_handler(USBHS_IRQn, usbhd_irq);
  enable_usbd_debug();
  irq_enable_interrupt(USBHS_IRQn);
  USBHD->CONTROL |= USBHD_CONTROL_DEV_PU_EN;  
}    
