#include "../../spi_device.h"

// clang-format off
const bool spi_instantiated[3] = { 1, 0, 0 };
SPI_Instance SPI_Instances[3] = {
  {
    .base = SPI1_BASE,
    .rcc_devid = RCC_SPI1EN,
    .irqn = SPI1_IRQn,
    .enabled = false,
    .configured = false,
    .chosen_pinconfig = SPI1_DEFAULT_MAPPING,
    .no_pin_configs = SPI1_NO_OF_PINCONFIGS,
    .pin_configuration = {
      /*NSS  SCK MISO  MOSI */
      { PA4, PA5, PA6, PA7 }, /* Default configuration */
      { PA15, PB3, PB4, PB5 } /* Alternate configuration */
    },
  },
  {
    .base = SPI2_BASE,
    .rcc_devid = RCC_SPI2EN,
    .irqn = SPI2_IRQn,
    .enabled = false,
    .configured = false,
    .chosen_pinconfig = SPI2_DEFAULT_MAPPING,
    .no_pin_configs = SPI2_NO_OF_PINCONFIGS,
    .pin_configuration = {
      /* NSS  SCK   MISO  MOSI */
      { PB12, PB13, PB14, PB15},
      {},
    },
  },
  {
    .base = SPI3_BASE,
    .rcc_devid = RCC_SPI3EN,
    .irqn = SPI3_IRQn,
    .enabled = false,
    .configured = false,
    .chosen_pinconfig = SPI3_DEFAULT_MAPPING,
    .no_pin_configs  =SPI3_NO_OF_PINCONFIGS,
    .pin_configuration = {
      /* NSS  SCK  MISO MOSI */
      { PA15, PB3, PB4, PB5 },   /* Default configuration */
      { PA4, PC10, PC11, PC12 }, /* Alternate configuration */
    },
  },
};
