#ifndef SRC_RCC_DEVICE_H
#define SRC_RCC_DEVICE_H
#include <stdint.h>
#include <stddef.h>

/* RCC Register definitions */
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


#define RCC_BASE               ((uintptr_t)0x40021000)
#define RCC                    ((RCC_Regfile *)RCC_BASE)

#endif
