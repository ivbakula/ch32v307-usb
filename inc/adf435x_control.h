#ifndef INC_ADF435x_CONTROL_H
#define INC_ADF435x_CONTROL_H

#include "gpio_interface.h"
#include "spi_interface.h"

typedef enum
{
  ADF435x_Err_Success,
  ADF435x_Err_ConfigFail,
  ADF435x_Err_NoSuchDevice,
} ADF435x_Err;

typedef struct
{
  uint32_t adf435x_regs[6];
} ADF435x_Regs;

typedef struct
{
  GPIO_Pin LE; /*< Load enable. On LE posedge, bits transfered by SPI are loaded into desired register. */
  GPIO_Pin CE; /*< Chip Enable. When high, ADF435x is turned ON */
  GPIO_Pin LD; /*< Lock detect. */
} ADF435x_PinConfig;

/**
 * @fn adf435x_configure_interface
 *
 * @brief
 *
 * @param spi_bus: SPI device that ADF435x is connected to.
 */
ADF435x_Err adf435x_create_interface(SPI_Device spi_bus, ADF435x_PinConfig pin_config);

/**
 * @fn adf435x_program_device
 *
 * @brief Write to ADF435x configuration registers over SPI bus.
 *
 * @param ADF435x_Regs Desired values of ADF435x registers
 */
ADF435x_Err adf435x_program_device(ADF435x_Regs regs);

#endif
