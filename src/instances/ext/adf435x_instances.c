#include "../../adf435x_device.h"

// clang-format off
const size_t adf435x_noof_instances = 1;
const bool adf435x_instantiated[1] = { 1 };
ADF435x_Instance ADF435x_Instances[1] = {
  {
    .uuid = 0xdeadbeef,
    .spi_bus = 0,
    .is_binded = false,
    .is_enabled = false,
    .spi_transfer_size = 0,
    .no_pin_config = ADF435x_MAX_PINCONFIG,
    .chosen_pinconfig = ADF435x_PinConfig_Default,
    .pin_configuration = {
     /* LE   CE   LD  */
      { PA1, PA0, PA2 },
    },
  },
};
