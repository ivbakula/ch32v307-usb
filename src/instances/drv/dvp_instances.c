#include "../../dvp_device.h"

// clang-format off
const bool dvp_instantiated[1] = {1};
DVP_Instance DVP_Instances[1] = {
  .base = DVP_BASE,
  .rcc_devid = RCC_DVPEN,
  .irqn = DVP_IRQn,
  .enabled = false,
  .configured = false,
  .chosen_pinconfig = DVP_DEFAULT_PINCONFIG,
  .pin_configuration = {
    { PA4, PA5, PA6, PA9, PA10, PC8, PC9, PC11, PB6, PB8, PB9, PC10, PC12, PD6, PD2 },
    { PA4, PA5, PA6, PA9, PA10, PC8, PC9, PC11, PB3, PB8, PB9, PC10, PC12, PD6, PD2 },
  },
};
