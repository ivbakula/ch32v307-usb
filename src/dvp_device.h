#ifndef SRC_DVP_DEVICE_H
#define SRC_DVP_DEVICE_H

#include <stddef.h>
#include <stdint.h>

#include "dvp_interface.h"
#include "gpio_interface.h"
#include "irq.h"

typedef struct
{
  volatile uint8_t R8_DVP_CR0;
  volatile uint8_t R8_DVP_CR1;
  volatile uint8_t R8_DVP_IER;
  volatile uint8_t padding_1;
  volatile uint16_t R16_DVP_ROW_NUM;
  volatile uint16_t R16_DVP_COL_NUM;
  volatile uint32_t R32_DVP_DMA_BUF0;
  volatile uint32_t R32_DVP_DMA_BUF1;
  volatile uint8_t R8_DVP_IFR;
  volatile uint8_t R8_DVP_STATUS;
  volatile uint16_t padding_2;
  volatile uint16_t R16_DVP_ROW_CNT;
  volatile uint16_t padding_4;
  volatile uint16_t R16_DVP_HOFFCNT;
  volatile uint16_t R16_DVP_VST;
  volatile uint16_t R16_DVP_CAPCNT;
  volatile uint16_t R16_DVP_VLINE;
  volatile uint32_t R32_DVP_DR;
} DVP_Regfile;

#define DVP_BASE ((uintptr_t)0x50050000)

#define DVP_MAX_PINCONFIGS 2

typedef struct
{
  uintptr_t base;
  RCC_DevId rcc_devid;
  Interrupt_IRQn irqn;
  bool enabled;
  bool configured;

  uint8_t chosen_pinconfig;
  uint8_t no_pin_configs;
  GPIO_Pin pin_configuration[DVP_MAX_PINCONFIGS][16];
} DVP_Instance;

#define DVP                 ((DVP_Regfile *)DVP_BASE)
#define _DVP_REGISTER(X, Y) ((uintptr_t) & (((DVP_Regfile *)X)->Y))

#endif
