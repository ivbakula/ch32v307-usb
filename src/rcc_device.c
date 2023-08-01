#include "rcc_device.h"
#include "rcc_interface.h"
#include "rcc_device.h"
#include "mmio_ops.h"

#ifdef UNITTEST
RCC_Regfile rcc_regfile; /*< Used for mocking of RCC registers */
#endif

static uintptr_t get_periph_rcc_register(uint32_t rcc_devid_mask)
{
  switch (rcc_devid_mask)
    {
    case RCC_AHB_DEVICE_MASK:
      return _RCC_REGISTER(R32_RCC_AHBPCENR);
    case RCC_APB1_DEVICE_MASK:
      return _RCC_REGISTER(R32_RCC_APB1PCENR);
    case RCC_APB2_DEVICE_MASK:
      return _RCC_REGISTER(R32_RCC_APB2PCENR);
    }

  // invalid periph mask
  return 0;
}

void rcc_pcendis(RCC_DevId devid, bool on)
{
  uint32_t device = devid & (~RCC_PERIPH_BANK_MASK);
  uint32_t rcc_bank_mask = devid & RCC_PERIPH_BANK_MASK;
  uintptr_t bank_register = get_periph_rcc_register(rcc_bank_mask);

  if (on) {
    mmio_or_writedw(bank_register, device);
  } else {
    mmio_and_writedw(bank_register, ~device);
  }
}
