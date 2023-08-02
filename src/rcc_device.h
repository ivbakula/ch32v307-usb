#ifndef SRC_RCC_DEVICE_H
#define SRC_RCC_DEVICE_H
#include <stddef.h>
#include <stdint.h>

#include "rcc_interface.h"

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
  volatile uint32_t R32_RCC_BDCTRL;
  volatile uint32_t R32_RCC_RSTSCKR;
  volatile uint32_t R32_RCC_AHBRSTR;
  volatile uint32_t R32_RCC_CFGR2;
} RCC_Regfile;

#ifndef UNITTEST
#define RCC_BASE ((uintptr_t)0x40021000)
#else
extern RCC_Regfile rcc_regfile;
#define RCC_BASE ((uintptr_t)&rcc_regfile)
#endif

/* This macro is probably very very bad idea */
#define _RCC_REGISTER(y) ((uintptr_t) & (((RCC_Regfile *)RCC_BASE)->y))

#endif
