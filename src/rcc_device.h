#ifndef SRC_RCC_DEVICE_H
#define SRC_RCC_DEVICE_H
#include <stdint.h>
#include <stddef.h>
#
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

#ifndef UNITTEST
#define RCC_BASE ((uintptr_t)0x40021000)
#else
static RCC_Regfile rcc_regfile;
#define RCC_BASE ((uintptr_t) &rcc_regfile)
#endif

#define CTRL (RCC_BASE + offsetof(RCC_Regfile, R32_RCC_CTRL))
#define CFGR0 (RCC_BASE + offsetof(RCC_Regfile, R32_RCC_CFGR0))
#define INTR  (RCC_BASE + offsetof(RCC_Regfile, R32_RCC_INTR))
#define APB2PRSTR (RCC_BASE + offsetof(RCC_Regfile, R32_RCC_APB2PRSTR))
#define APB1PRSTR (RCC_BASE + offsetof(RCC_Regfile, R32_RCC_APB1PRSTR))
#define AHBPCENR (RCC_BASE + offsetof(RCC_Regfile, R32_RCC_AHBPCENR))
#define APB2PCENR (RCC_BASE + offsetof(RCC_Regfile, R32_RCC_APB2PCENR))
#define APB1PCENR (RCC_BASE + offsetof(RCC_Regfile, R32_RCC_APB1PCENR))
#define APB


#endif
