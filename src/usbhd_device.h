#ifndef SRC_USBHD_DEVICE_H
#define SRC_USBHD_DEVICE_H

#include "usbhd_interface.h"
#include "irq.h"
#include "rcc_interface.h"
#include "gpio_interface.h"

typedef struct
{
  uintptr_t base;
  RCC_DevId rcc_devid;
  Interrupt_IRQn irqn;
  bool enabled;
  bool configured;
  GPIO_Pin pin_configuration[1][2];
} USBHD_Instance;


#endif
