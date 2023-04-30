#include "ch32v307.h"
#include "time.h"
#include "irq.h"

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

extern void uart_puts(const char *str);
static void usbhd_irq(void)
{
  uart_puts(__FUNCTION__);
  if (USBHD->INT_FG & USBHD_INTFG_SETUP_ACT)
    uart_puts("USBHD_INTFG_SETUP_ACT\n");
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
  USBHD->CONTROL = USBHD_CONTROL_DMA_EN;

  USBHD->INT_EN = U32_BIT(0) | U32_BIT(1) | U32_BIT(5);
  USBHD->UEP0_MAX_LEN = DEF_USBD_UEP0_SIZE;
  USBHD->UEP0_DMA = (uint32_t)((uint8_t *)USBHS_EP0_Buf);
  USBHD->UEP0_TX_LEN = 0;
  USBHD->UEP0_TX_CTRL = 0x02;
  USBHD->UEP0_RX_CTRL = 0x00;
  USBHD->CONTROL |= USBHD_CONTROL_DEV_PU_EN;
  
  irq_register_interrupt_handler(USBHS_IRQn, usbhd_irq);
  irq_enable_interrupt(USBHS_IRQn);
}    
