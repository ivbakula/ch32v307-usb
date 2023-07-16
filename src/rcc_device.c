#include "rcc_device.h"
#include "rcc_interface.h"

static inline void rcc_manipulate_periph(uint32_t periph, uint32_t mask, uint32_t device)
{
  switch (periph) {
    case RCC_AHB_DEVICE_MASK:
      RCC->R32_RCC_AHBPCENR |= device
      break;
      
    case RCC_APB1_DEVICE_MASK:
      RCC->R32_RCC_APB1PCENR |= device;
      break;

    case RCC_APB2_DEVICE_MASK:
      RCC->R32_RCC_APB2PCENR |= device;
      break;
    default:
      // not implemented
      break;
  }
}
void rcc_enable_periph(RCC_DevId rcc_devid)
{
  uint32_t device = rcc_devid & (~RCC_DEVICE_EN_MASK);
  uint32_t periph = rcc_devid & RCC_DEVICE_EN_MASK;
  rcc_manipulate_periph(periph, device, device);
}

void rcc_disable_periph(RCC_DevId rcc_devid)
{
  uint32_t device = rcc_devid & (~RCC_DEVICE_EN_MASK);
  uint32_t periph = rcc_devid & RCC_DEVICE_EN_MASK;
  rcc_manipulate_periph(periph, device, ~device);
}  
