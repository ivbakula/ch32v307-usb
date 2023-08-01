#ifndef CH32V307_H
#define CH32V307_H

#include <stdint.h>
#define APB_CLOCK 8000000  // Peripherial clock. This is the default value

/* Helper macros */
#define U8_BIT(x)  ((uint8_t)(1 << x))
#define U16_BIT(x) ((uint16_t)(1 << x))
#define U32_BIT(x) ((uint32_t)(1 << x))

#define DIV_4(x) (x >> 2)
#define MUL_4(x) (x << 2)

/* RCC stuff */
typedef struct
{
  volatile uint32_t R32_RCC_CTRL;
  volatile uint32_t R32_RCC_CFGR0;
  volatile uint32_t R32_RCC_INTR;
  volatile uint32_t R32_RCC_APB2PRSTR;
  volatile uint32_t R32_RCC_APB1PRSTR;
  volatile uint32_t R32_RCC_AHBPCENR;
  volatile uint32_t R32_RCC_APB2PCENR;
  volatile uint32_t R32_RCC_APB1PCENR;
  volatile uint32_t R32_RCC_BDCTLR;
  volatile uint32_t R32_RCC_RSTSCKR;
  volatile uint32_t R32_RCC_AHBRSTR;
  volatile uint32_t R32_RCC_CFGR2;
} RCC_Regfile;

#define RCC_BASE               ((uint32_t)0x40021000)
#define RCC_APB2PCENR_IOPAEN   U32_BIT(2)
#define RCC_APB2PCENR_IOPBEN   U32_BIT(3)
#define RCC_APB2PCENR_USART1EN U32_BIT(14)
#define RCC_AHBPCENR_USBHSEN   U32_BIT(11)
#define RCC_CTRL_PLLON         U32_BIT(24)
#define RCC_CTRL_PLLRDY        U32_BIT(25)
#define RCC_CTRL_HSEON         U32_BIT(16)
#define RCC_CTRL_HSERDY        U32_BIT(17)

#define RCC_CFGR0_PLLXPRE_DIV2 U32_BIT(17)
#define RCC_CFGR0_PLLSRC_HSE   U32_BIT(16)
#define RCC_CFGR0_SW_PLL       0b10
#define RCC_CFGR0_SWS_PLL      ((uint32_t)0b10 << 2)
#define RCC_CFGR0_PLLMUL_6     ((uint32_t)0b0100 << 18)

#define RCC_CFGR2_USBHSSRC_USBPHY U32_BIT(31)
#define RCC_CFGR2_USBHSPLL_EN     U32_BIT(30)

#define RCC_CFGR2_USBHSCLK_3MHz ((uint32_t)(0b00 << 28))
#define RCC_CFGR2_USBHSCLK_4MHz ((uint32_t)(0b01 << 28))
#define RCC_CFGR2_USBHSCLK_8MHz ((uint32_t)(0b10 << 28))
#define RCC_CFGR2_USBHSCLK_5MHz ((uint32_t)(0b11 << 28))

#define RCC ((RCC_Regfile *)RCC_BASE)

/* GPIO stuff */
typedef struct
{
  volatile uint32_t R32_GPIO_CFGLR;
  volatile uint32_t R32_GPIO_CFGHR;
  volatile uint32_t R32_GPIO_INDR;
  volatile uint32_t R32_GPIO_OUTDR;
  volatile uint32_t R32_GPIO_BSHR;
  volatile uint32_t R32_GPIO_BCR;
  volatile uint32_t R32_GPIO_LCKR;
} GPIO_Regfile;

#define GPIO_PUSH_PULL_OUTPUT_MODE       0b0000
#define GPIO_GENERAL_OPEN_DRAIN_OUTPUT   0b0100
#define GPIO_PUSH_PULL_ALTERNATE_OUTPUT  0b1000
#define GPIO_OPEN_DRAIN_ALTERNATE_OUTPUT 0b1100

#define GPIO_OUTPUT_SPEED_10MHZ 0b0001
#define GPIO_OUTPUT_SPEED_2MHZ  0b0010
#define GPIO_OUTPUT_SPEED_50MHZ 0b0011

// #define GPIO_PUSH_PULL_ALTERNATE_OUTPUT 0b1001
#define GPIO_PULL_UP_INPUT 0b1000

#define GPIOA_BASE ((uint32_t)0x40010800)
#define GPIOB_BASE ((uint32_t)0x40010C00)
#define GPIOA      ((GPIO_Regfile *)GPIOA_BASE)
#define GPIOB      ((GPIO_Regfile *)GPIOB_BASE)

/* USART stuff */
typedef struct
{
  volatile uint32_t R32_USART_STATR;
  volatile uint32_t R32_USART_DATAR;
  volatile uint32_t R32_USART_BRR;
  volatile uint32_t R32_USART_CTRL1;
  volatile uint32_t R32_USART_CTRL2;
  volatile uint32_t R32_USART_CTRL3;
  volatile uint32_t R32_USART_GPR;
} USART_Regfile;

#define USART_STATR_TC     U32_BIT(6)
#define USART_STATR_TXE    U32_BIT(7)
#define USART_CTRL1_RE     U32_BIT(2)
#define USART_CTRL1_TE     U32_BIT(3)
#define USART_CTRL1_UE     U32_BIT(13)
#define USART_CTRL1_RXNEIE U32_BIT(5)

#define USART1_BASE ((uint32_t)0x40013800)
#define USART1      ((USART_Regfile *)USART1_BASE)

typedef struct
{
  volatile uint16_t CTLR1;
  volatile uint16_t padding_1;
  volatile uint16_t CTLR2;
  volatile uint16_t padding_2;
  volatile uint16_t STATR;
  volatile uint16_t padding_3;
  volatile uint16_t DATAR;
  volatile uint16_t padding_4;
  volatile uint16_t CRCR;
  volatile uint16_t padding_5;
  volatile uint16_t RCRCR;
  volatile uint16_t padding_6;
  volatile uint16_t TCRCR;
  volatile uint16_t padding_7;
  volatile uint16_t I2S_CFGR;
  volatile uint16_t padding_8;
  volatile uint16_t HSCR;
} SPI_Regfile;

#define SPI1_BASE ((uintptr_t)0x40013000)
#define SPI2_BASE ((uintptr_t)0x40003800)
#define SPI1      ((SPI_Regfile *)SPI1_BASE)
#define SPI2      ((SPI_Regfile *)SPI2_BASE)

/* IRQ stuff */
typedef struct
{
  volatile uint32_t R32_PFIC_ISR1;
  /* TODO add everything else */

} PFIC_Regfile;

#define PFIC_BASE ((uint32_t)0xE000E000)
#define PFIC      ((PFIC_Regfile *)PFIC_BASE)

typedef enum
{
  IRQ_SysTick = 12
} IRQ_Line;

/* Extended configuration stuff */
#define EXTCFG_BASE ((uint32_t)0x40023800)

#define EXTCFG_USBDLS_LSM U32_BIT(0) /* USBD low speed mode */
#define EXTCFG_USBDPU_EN  U32_BIT(1) /* USBD enable internal pull-up resistor */
#define EXTCFG            (*((uint32_t *)EXTCFG_BASE))
#endif
