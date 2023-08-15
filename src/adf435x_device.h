#ifndef SRC_ADF435x_INSTANCE_H
#define SRC_ADF435x_INSTANCE_H

#include <stddef.h>

#include "adf435x_interface.h"

#define ADF435x_MAX_PINCONFIG 1

typedef struct
{
  uint32_t uuid;
  SPI_Device spi_bus; /*< SPI interface that ADF435x device is connected to */
  bool is_binded;
  bool is_enabled;
  size_t spi_transfer_size;
  const uint8_t no_pin_config;
  ADF435x_PinConfig chosen_pinconfig;
  const GPIO_Pin pin_configuration[1][3];
} ADF435x_Instance;

typedef union
{
  uint8_t r8[4];   /* use this array if spi_transfer_size == SPI_PACKET_TRANSFER_SIZE_8 */
  uint16_t r16[2]; /* use this array if spi_transfer_size == SPI_PACKET_TRANSFER_SIZE_16 */

  uint32_t reg; /*  Whole register */
} ADF435x_RegisterHelper;


#define ADF435x_LE_INDEX 0 /*< Load enable. On LE posedge, bits transfered by SPI are loaded into desired register. */
#define ADF435x_CE_INDEX 1 /*< Chip Enable. When high, ADF435x is turned ON */
#define ADF435x_LD_INDEX 2 /*< Lock detect. */

#endif
