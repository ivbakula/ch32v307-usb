#include "rcc_device.h"
#include "rcc_interface.h"

static inline void rcc_manipulate_periph(uint32_t periph, uint32_t mask, uint32_t device)
{
  switch (periph) {
    case RCC_AHB_DEVICE_MASK:
      mmio_write_wmask_u32(R32_RCC_AHBPCENR, mask, device);
      break;
      
    case RCC_APB1_DEVICE_MASK:
      mmio_write_wmask_u32(R32_RCC_APB1PCENR, mask, device);
      break;

    case RCC_APB2_DEVICE_MASK:
      mmio_write_wmask_u32(R32_RCC_APB2PCENR, mask, device);
      break;
    default:
      // not implemented
      break;
  }
}
void rcc_enable_periph(uint32_t sel)
{
  uint32_t device = sel & (~RCC_DEVICE_EN_MASK);
  uint32_t periph = sel & RCC_DEVICE_EN_MASK;
  rcc_manipulate_periph(periph, device, device);
}

void rcc_disable_periph(uint32_t sel)
{
  uint32_t device = sel & (~RCC_DEVICE_EN_MASK);
  uint32_t periph = sel & RCC_DEVICE_EN_MASK;
  rcc_manipulate_periph(periph, device, ~device);
}  
