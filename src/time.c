#include "time.h"

#include <stdint.h>

#include "ch32v307.h"

typedef struct
{
  volatile uint32_t R32_STK_CTRL;
  volatile uint32_t R32_STK_SR;
  volatile uint32_t R32_STK_CNTL;
  volatile uint32_t R32_STK_CNTH;
  volatile uint32_t R32_STK_CMPLR;
  volatile uint32_t R32_STK_CMPHR;
} SysTick_Regfile;

#define SYSTICK_BASE       ((uint32_t)0xE000F000)
#define SYSTICK_CTRL_STE   U32_BIT(0)
#define SYSTICK_CTRL_STIE  U32_BIT(1)
#define SYSTICK_CTRL_STCLK U32_BIT(2)
#define SYSTICK_CTRL_STRE  U32_BIT(3)
#define SYSTICK_CTRL_MODE  U32_BIT(4)
#define SYSTICK_CTRL_INIT  U32_BIT(5)
#define SYSTICK_CTRL_SWIE  U32_BIT(31)
#define _PLLMULF(x)        (x << 18)

#define SYSTICK ((SysTick_Regfile *)SYSTICK_BASE)

uint32_t sysclock_frequency = HSI_CLOCK_FREQUENCY;

static void delay(uint32_t n)
{
  SYSTICK->R32_STK_SR &= ~U32_BIT(0);
  SYSTICK->R32_STK_CMPLR = n;
  SYSTICK->R32_STK_CMPHR = 0;

  SYSTICK->R32_STK_CNTL = 0;
  SYSTICK->R32_STK_CNTH = 0;

  SYSTICK->R32_STK_CTRL |= SYSTICK_CTRL_STE | SYSTICK_CTRL_STCLK;

  while (!(SYSTICK->R32_STK_SR & U32_BIT(0)))
    ;

  SYSTICK->R32_STK_CTRL &= ~(SYSTICK_CTRL_STE | SYSTICK_CTRL_STCLK);
}

void wait_ms(uint32_t ms)
{
  uint32_t N = 48000;  // number of ticks for one milisecond per 48MHz clock frequency. Hardcoded for now
  uint32_t n = N * ms;

  delay(n);
}

void wait_us(uint32_t us)
{
  uint32_t N = 48;
  uint32_t n = N * us;
  delay(n);
}

static uint8_t pllmul_factor[] = {18, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
  7, /* this is not correct. This should be 6.5 but leave it as it is now */
  15, 16};

void system_pll_clock_init(PLLMul mul)
{
  /* Enable HSE clock */
  RCC->R32_RCC_CTRL |= RCC_CTRL_HSEON;

  /* Wait for HSE ready */
  while (!(RCC->R32_RCC_CTRL & RCC_CTRL_HSERDY))
    ;

  /* Configure PLL clock. BIG NOTE: If you want to use 8MHz HSI clock
   * as a reference clock, please note that, by default, this clock is divided
   * by 2 (which gives 4MHz reference clock at input of PLL). To change this,
   * you should modify EXTEN_CTR register. This register contains field which
   * you should set in order to remove divide by two. Field in question is called
   * HSIPRE (4th bit of this register). See CH32FV2x_V3xRM-EN pg 583 */
  RCC->R32_RCC_CFGR0 |= RCC_CFGR0_PLLSRC_HSE | _PLLMULF(mul);

  /* Enable PLL */
  RCC->R32_RCC_CTRL |= RCC_CTRL_PLLON;

  /* Wait for PLL to lock */
  while (!(RCC->R32_RCC_CTRL & RCC_CTRL_PLLRDY))
    ;

  /* Switch to PLL clock */
  RCC->R32_RCC_CFGR0 |= RCC_CFGR0_SW_PLL;

  /* Wait for system clock */
  while (!(RCC->R32_RCC_CFGR0 & RCC_CFGR0_SWS_PLL))
    ;

  sysclock_frequency = HSE_CLOCK_FREQUENCY * pllmul_factor[mul];
}
