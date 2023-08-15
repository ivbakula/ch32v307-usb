#ifndef INC_ADF435x_CONTROL_H
#define INC_ADF435x_CONTROL_H

#include "gpio_interface.h"
#include "spi_interface.h"

typedef enum
{
  ADF435x_Err_Success,
  ADF435x_Err_ConfigFail,
  ADF435x_Err_NoSuchDevice,
  ADF435x_Err_NotBinded,
  ADF435x_Err_NotEnabled,
} ADF435x_Err;

typedef struct
{
  uint32_t adf435x_regs[6];
} ADF435x_Regs;

typedef enum
{
  ADF435x_Device1,
} ADF435x_Device;

typedef enum
{
  ADF435x_PinConfig_Default,
} ADF435x_PinConfig;

/**
 * @fn adf435x_bind_device
 *
 * @brief Bind ADF435x device to specific SPI bus and GPIO pins.
 *
 * @param dev: Desired ADF435x device
 *
 * @param spi_bus: SPI bus that ADF435x is connected to.
 *
 * @param pin_config: GPIO pins dedicated to ADF435x functionalities.
 */
ADF435x_Err adf435x_bind_device(ADF435x_Device dev, SPI_Device spi_bus, ADF435x_PinConfig pin_config);

/**
 * @fn adf435x_enable_device
 *
 * @brief Enables ADF435x chip by pulling CE (chip enable) pin high.
 *
 * @param dev Desired ADF435x device
 */
ADF435x_Err adf435x_enable_device(ADF435x_Device dev);

/**
 * @fn adf435x_disable_device
 *
 * @brief Disables ADF435x chip by pulling CE (chip enable) pin low.
 *
 * @param dev Desired ADF435x device
 */
ADF435x_Err adf435x_disable_device(ADF435x_Device dev);

/**
 * @fn adf435x_program_device
 *
 * @brief Write to ADF435x configuration registers over SPI bus.
 *
 * @param dev ADF435x device to be programmed
 *
 * @param ADF435x_Regs Desired values of ADF435x registers
 */
ADF435x_Err adf435x_program_device(ADF435x_Device dev, ADF435x_Regs regs);

#endif
