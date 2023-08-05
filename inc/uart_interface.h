#ifndef INC_UART_INTERFACE_H
#define INC_UART_INTERFACE_H

#include <stdint.h>

typedef enum
{
  UART_Device1,
  UART_Device2,
  UART_Device3,
  UART_Device4,
  UART_Device5,
  UART_Device6,
  UART_Device7,
  UART_Device8,

  /* NOT RELEVANT FOR NOW; BUT IT MIGHT BE IN FUTURE.
   * Following definitons are not relevant, because at this time only UART
   * support is planned (asynchronous transfer only). First 4
   * devices support both UART and USART but those will be used
   * as plain 'ol UART only. Maybe in the future, there will be a need for USART but
   * right now, there is none
   */
  USART_Device1,
  USART_Device2,
  USART_Device3,
  USART_Device4,
} UART_Device;

typedef uint8_t UART_PinConfig;

/**
 * @section U(S)ART_pin_configuration
 * @brief This section defines macros that will be used for selecting desired
 *        pin configuration for selected U(S)ART device. For more details about
 *        (alternate) pin configurations, check out section 10.2.11.4. of Reference manual.
 *        ch32v307-usb/doc/CH32FV2x_V3xRM.PDF
 */
///@{

/**
 * @name USART1_DEFAULT_MAPPING
 *
 * @brief Default PIN mapping for USART1.
 * 
 * |------+-----+-----+------+------+------+
 * | FUNC | CK  | TX  | RX   | CTS  | RTS  |
 * |------+-----+-----+------+------+------+
 * | PIN  | PA8 | PA9 | PA10 | PA11 | PA12 |
 * |------+-----+-----+------+------+------|
 * 
 */
#define USART1_DEFAULT_MAPPING ((UART_PinConfig) 0b00)

/**
 * @name USART1_ALTERNATE_MAPPING_1
 *
 * @brief First alternative pin mapping for USART1
 * 
 * |------+-----+-----+-----+------+------|
 * | FUNC | CK  | TX  | RX  | CTS  | RTS  |
 * |------+-----+-----+-----+------+------|
 * | PIN  | PA8 | PB6 | PB7 | PA11 | PA12 |
 * |------+-----+-----+-----+------+------|
 * 
 */
#define USART1_ALTERNATE_MAPPING_1 ((UART_PinConfig)0b01)

/**
 * @name USART1_ALTERNATE_MAPPING_2
 *
 * @brief Second alternative pin mapping for USART1
 *
 * |------+------+------+-----+-----+-----|
 * | FUNC | CK   | TX   | RX  | CTS | RTS |
 * |------+------+------+-----+-----+-----|
 * | PIN  | PA10 | PB15 | PA8 | PA5 | PA9 |
 * |------+------+------+-----+-----+-----|
 * 
 */
#define USART1_ALTERNATE_MAPPING_2 ((UART_PinConfig)0b10)

/**
 * @name USART1_ALTERNATE_MAPPING_3
 *
 * @brief Third alternate pin mapping for USART1
 *
 * |------+-----+-----+-----+-----+-----|
 * | FUNC | CK  | TX  | RX  | CTS | RTS |
 * |------+-----+-----+-----+-----+-----|
 * | PIN  | PA5 | PA6 | PA7 | PC4 | PC5 |
 * |------+-----+-----+-----+-----+-----|
 * 
 */
#define USART1_ALTERNATE_MAPPING_2 ((UART_PinConfig)0b11)
///@}

typedef enum
{
  UART_Err_Success,
  UART_Err_ConfigFail,
  UART_Err_NotEnabled,
  UART_Err_AlreadyEnabled,
} UART_Err;

/**
 * @fn uart_enable_device
 *
 * @brief This function enables peripherial clock for:
 *             a) desired U(S)ART device
 *             b) GPIO port(s) for given pin configuration
 *             c) Configures GPIO pins in accordance with selected pin configuration
 *
 * @param dev: uart device to enable
 * 
 * @param pin_config: U(S)ART device pin configuration
 * 
 * @return UART_Err_Success if successful, appropriate error code otherwise. 
 */
UART_Err uart_enable_device(UART_Device dev, UART_PinConfig pin_config);

/**
 * @fn uart_disable_device
 *
 * @brief Disable uart and deallocate all resources that were associated with
 *        selected device. In other words, this function will do the following:
 *        1) Disable peripherial clock to selected UART device
 *        2) Unlock all pins that were locked by /uart_enable_device/
 *
 * @param dev: UART device to disable
 *
 * @return UART_Err_Success if successful, appropriate error code otherwise.
 */
UART_Err uart_disable_device(UART_Device dev);

/**
 * @fn uart_reset_device
 *
 * @brief Resets UART device in RCC.
 *
 * @param dev UART device to reset
 *
 * @return UART_Err_Success if successful, appropriate error code otherwise.
 * 
 */
UART_Err uart_reset_device(UART_Device dev);

/**
 * @fn uart_configure_device
 *
 * @brief This function configures UART device. NOTE: This function shall be called only once after succssful
 *        configuration of UART device. For any subsequent reconfiguration, the device shall be restarted
 *        beforehand.
 *
 * @param dev: UART device to configure
 *
 * @param baud: UART baud rate
 *
 * @param apbclk: Main system clock speed in Hz.
 */
UART_Err uart_configure_device(UART_Device dev, uint32_t baud, uint32_t apbclk);

/**
 * @fn uart_putc
 *
 * @brief Output one character to desired UART device. NOTE: This is blocking function; If output buffer
 *        is full, it will block until it's empty.
 *
 * @param dev: Desired UART device
 *
 * @param c: Character for output
 */
void uart_putc(UART_Device dev, char c);

/**
 * @fn uart_getc
 *
 * @brief Read one character from UART device. NOTE: This is blocking function; If there are no
 *        available characters in UART buffer, this function will wait until there is.
 *
 * @param dev: Desired UART device
 *
 * @return character received on UART device.
 */
char uart_getc(UART_Device dev);

#endif
