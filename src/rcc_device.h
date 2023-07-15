#ifndef SRC_RCC_DEVICE_H
#define SRC_RCC_DEVICE_H
#include <stdint.h>
#include <stddef.h>

/* RCC Register definitions */
#define R32_RCC_CTLR ((uintptr_t) 0x40021000)
#define R32_RCC_CFGR0 ((uintptr_t)0x40021004)
#define R32_RCC_INTR  ((uintptr_t)0x4002100C)
#define R32_RCC_APB1PRSTR ((uintptr_t)0x40021010)
#define R32_RCC_AHBPCENR  ((uintptr_t)0x40021014)
#define R32_RCC_APB2PCENR ((uintptr_t)0x40021018)
#define R32_RCC_APB1PCENR ((uintptr_t)0x4002101C)
#define R32_RCC_BDCTLR    ((uintptr_t)0x40021020)
#define R32_RCC_RSTSCKR   ((uintptr_t)0x40021024)
#define R32_RCC_AHBRSTR   ((uintptr_t)0x40021028)
#define R32_RCC_CFGR2     ((uintptr_t)0x4002102C)

#endif
