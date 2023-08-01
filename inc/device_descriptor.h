#ifndef INC_DEVICE_DESCRIPTOR_H
#define INC_DEVICE_DESCRIPTOR_H

#include <stdbool.h>

#include "rcc_devids.h"

typedef enum
{
  DEV_USART,
  DEV_UART,
  DEV_SPI,
  DEV_USB,
  // TODO add every possible device
  DEV_MAX
} DeviceId;

typedef struct
{
  uintptr_t mmio_base;
  DeviceId device_id;
  RCCId rcc_id;
  bool ready;
  uint8_t instance;
} DeviceDescriptor;

#endif
