#ifndef INC_SPI_INTERFACE_H
#define INC_SPI_INTERFACE_H

#include <stdint.h>

typedef enum
{
  SPI_Device1,
  SPI_Device2,
  SPI_Device3,
  SPI_Device_MAX,
} SPI_Device;

typedef uint8_t SPI_PinConfig;

/**
 * @section SPI_pin_configuration
 * @brief This section defines macros that will be used for selecting desired
 *        pin configuration for selected SPI device. For more details about
 *        (alternate) pin configurations, check out section 10.2.11.4. of Reference manual.
 *        ch32v307-usb/doc/CH32FV2x_V3xRM.PDF
 */
///@{

/**
 * @name SPI1_DEFAULT_MAPPING
 *
 * @brief Default PIN mapping for SPI1
 *
 * |------+-----+-----+------+------|
 * | FUNC | NSS | SCK | MISO | MOSI |
 * |------+-----+-----+------+------|
 * | PIN  | PA4 | PA5 | PA6  | PA7  |
 * |------+-----+-----+------+------|
 *
 */
#define SPI1_DEFAULT_MAPPING ((SPI_PinConfig)0b0)


/**
 * @name SPI1_ALTERNATE_MAPPING
 *
 * @brief Alternate PIN mapping for SPI1
 *
 * |------+------+-----+------+------|
 * | FUNC | NSS  | SCK | MISO | MOSI |
 * |------+------+-----+------+------|
 * | PIN  | PA15 | PB3 | PB4  | PB5  |
 * |------+------+-----+------+------|
 *
 */
#define SPI1_ALTERNATE_MAPPING ((SPI_PinConfig)0b1)

/**
 * @name SPI2_DEFAULT_MAPPING
 *
 * @brief Default PIN mapping for SPI2
 *
 * |------+------+------+------+------|
 * | FUNC | NSS  | SCK  | MISO | MOSI |
 * |------+------+------+------+------|
 * | PIN  | PB12 | PB13 | PB14 | PB15 |
 * |------+------+------+------+------|
 * 
 */
#define SPI2_DEFAULT_MAPPING ((SPI_PinConfig)0b0)

/**
 * @name SPI3_DEFAULT_MAPPING
 *
 * @brief Default PIN mapping for SPI3
 *
 * |------+------+-----+------+------|
 * | FUNC | NSS  | SCK | MISO | MOSI |
 * |------+------+-----+------+------|
 * | PIN  | PA15 | PB3 | PB4  | PB5  |
 * |------+------+-----+------+------|
 *
 */
#define SPI3_DEFAULT_MAPPING ((SPI_PinConfig)0b0)

/**
 * @name SPI3_ALTERNATE_MAPPING
 *
 * @brief Alternate PIN mapping for SPI3
 *
 * |------+-----+------+------+------|
 * | FUNC | NSS | SCK  | MISO | MOSI |
 * |------+-----+------+------+------|
 * | PIN  | PA4 | PC10 | PC11 | PC12 |
 * |------+-----+------+------+------|
 *
 */
#define SPI3_ALTERNATE_MAPPING ((SPI_PinConfig)0b1)

///@}

/**
 * @section SPI_Configuration_Register_1
 *
 * @brief This section defines macros that will be used for configuring SPI device. This section in
 *        particular, covers options for SPIx_CTRL1 register. Macros with suffix _EN, enable specific feature,
 *        
 *        For more info about SPIx_CTRL1 register, check out ch32v307-usb/doc/CH32FV2x_V3xRM.PDF
 */
///@{
typedef uint16_t SPI_ConfigCtrl1;

/**
 * @name SPI_CTRL1_BIDIMODE_(EN|DS)
 *
 * @brief Unidirectional data mode enable. In other words, use only one line for communication. If unidirectional
 *        data mode is enabled, see SPI_CTRL1_BIDIOE_EN also.
 * 
 */
#define SPI_CTRL1_BIDIMODE_EN ((SPI_ConfigCtrl1)U32_BIT(15))

/**
 * @name SPI_CTRL1_BIDIOE_EN
 *
 * @brief Unidirectional data mode output enable; It should be used in conjunction with BIDIMODE.
 *
 */
#define SPI_CTRL1_BIDIOE_EN ((SPI_ConfigCtrl1)U32_BIT(14))

/**
 * @name SPI_CTRL1_CRCEN
 *
 * @brief Hardware CRC check enable. This bit can only be written when SPE is 0. Only full duplex mode is
 *        supported by HW CRC check
 * 
 */
#define SPI_CTRL1_HWCRC_EN ((SPI_ConfigCtrl1)U32_BIT(13))

/**
 * @name SPI_CTRL1_CRCNEXT
 *
 * @brief TODO Check Reference manual. I've never used this register
 *
 */
#define SPI1_CTRL1_CRCNEXT_1 ((SPI_ConfigCtrl1)U32_BIT(12))
#define SPI1_CTRL1_CRCNEXT_0 ((SPI_ConfigCtrl1)0)


/**
 * @name SPI_CTRL1_DFF
 *
 * @brief Data frame length;
 *
 * @suffix _8: 8 bit data frame length 
 * @suffix _16: 16 bit data frame length
 *
 */
#define SPI1_CTRL1_DFF_8 ((SPI_ConfigCtrl1)0)
#define SPI1_CTRL1_DFF_16 ((SPI_ConfigCtrl1)U16_BIT(11))

/**
 * @name SPI_CTRL1_RX_ONLY
 *
 * @brief Two wire simplex mode (Receive only)
 */
#define SPI_CTRL1_RX_ONLY ((SPI_ConfigCtrl1)U16_BIT(10))

/**
 * @name SPI_CTRL1_SSM_SW
 *
 * @brief CS Pin management. This macro will force Software control of the NSS pin
 *
 */
#define SPI_CTRL1_SSM_SW ((SPI_ConfigCtrl1)U16_BIT(9))

/**
 * @name SPI_CTRL1_NSS_LEVEL_MASK
 *
 * @brief Set logic level on the NSS PIN. By writing to this bit in SPI_CTRL1 register,
 *        NSS will be pulled high.
 */
#define SPI_CTRL1_NSS_LEVEL_MASK ((SPI_ConfigCtrl1)U16_BIT(8))

/**
 * @name
 *
 * @brief
 * 
 */
#define SPI_CTRL1_LSBFIRST ((SPI_ConfigCtrl1)U16_BIT(7))

/**
 * @name
 *
 * @brief
 */
#define SPI_CTRL1_MSBFIRST ((SPI_ConfigCtrl1)0)

/**
 * @name
 *
 * @brief
 */
#define SPI_CTRL1_SPIEN ((SPI_ConfigCtrl1)U16_BIT(6))


/**
 * @name SPI_CTRL1_BR_(2|4|8|16|32|64|128|256)
 *
 * @brief Baud rate setting domain, this register cannot be changed after SPI device
 *        is enabled.
 */
#define SPI_CTRL1_BR_FPCLK_2 ((SPI_ConfigCtrl1)0))
#define SPI_CTRL1_BR_FPCLK_4 ((SPI_ConfigCtrl1)(0b001 << 3))
#define SPI_CTRL1_BR_FPCLK_8 ((SPI_ConfigCtrl1)(0b010 << 3))
#define SPI_CTRL1_BR_FPCLK_16 ((SPI_ConfigCtrl1)(0b011 << 3))
#define SPI_CTRL1_BR_FPCLK_32 ((SPI_ConfigCtrl1)(0b100 << 3))
#define SPI_CTRL1_BR_FPCLK_64 ((SPI_ConfigCtrl1)(0b101 << 3))
#define SPI_CTRL1_BR_FPCLK_128 ((SPI_ConfigCtrl1)(0b110 << 3))
#define SPI_CTRL1_BR_FPCLK_256 ((SPI_ConfigCtrl1)(0b111 << 3))

/**
 * @name SPI_CTRL1_MASTER
 *
 * @brief Configure SPI device as Master device. This setting cannot be changed after SPI device is enabled
 * 
 */
#define SPI_CTRL1_MASTER ((SPI_ConfigCtrl1)U16_BIT(2))

/**
 * @name SPI_CTRL1_CPOL_(0|1)
 *
 * @brief Clock polarity selection.
 * _0: SCK remains low in idle state
 * _1: SCK remains high in idle state
 */
#define SPI_CTRL1_CPOL_1 ((SPI_ConfigCtrl1)U16_BIT(1))
#define SPI_CTRL1_CPOL_0 ((SPI_ConfigCtrl1)0)

/**
 * @name SPI_CTRL1_CPHA_(0|1)
 *
 * @brief Clock phase setting
 * _0: Data sampling starts from the first clock edge
 * _1: Data sampling starts from the second clock edge
 */
#define SPI_CTRL1_CPHA_1 ((SPI_ConfigCtrl1)U16_BIT(0))
#define SPI_CTRL1_CPHA_0 ((SPI_ConfigCtrl1)0)

///@}

typedef enum
{
  SPI_Err_Success,
} SPI_Err;

/**
 * @fn spi_enable_device
 *
 * @brief
 *
 * @param dev
 *
 * @param pin_config
 *
 * @return
 */
SPI_Err spi_enable_device(SPI_Device dev, SPI_PinConfig pin_config);

/**
 * @fn spi_disable_device
 *
 * @brief
 *
 * @param dev
 *
 * @return
 */
SPI_Err spi_disable_device(SPI_Device dev);

/**
 * @fn spi_reset_device
 *
 * @brief
 *
 * @param dev
 *
 * @return
 */
SPI_Err spi_reset_device(SPI_Device dev);

/**
 * @fn spi_configure_device
 *
 * @brief
 *
 * @param dev
 *
 * @param ctrl1
 */
SPI_Err spi_configure_device(SPI_Device dev, SPI_ConfigCtrl1 ctrl1_reg);

/**
 * @fn spi_write_u8
 *
 * @brief
 *
 * @param dev
 *
 * @param data
 */
void spi_write_u8(SPI_Device dev, uint8_t data);

/**
 * @fn spi_master_write_u16
 *
 * @brief
 *
 * @param dev
 *
 * @param data
 */
void spi_write_u16(SPI_Device dev, uint16_t data);

#endif
