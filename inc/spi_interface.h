#ifndef INC_SPI_INTERFACE_H
#define INC_SPI_INTERFACE_H

#include <stdbool.h>
#include <stdint.h>

#define SPI_PACKET_TRANSFER_SIZE_8 (sizeof(uint8_t))
#define SPI_PACKET_TRANSFER_SIZE_16 (sizeof(uint16_t))
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

#define SPI1_NO_OF_PINCONFIGS 2

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

#define SPI2_NO_OF_PINCONFIGS 1
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

#define SPI3_NO_OF_PINCONFIGS 2

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

typedef struct
{
  union CTRL1
  {
    /* SPI_CTRL1 */
    struct
    {
      uint16_t _CPHA : 1;      /*< Clock phase */
      uint16_t _CPOL : 1;      /*< Clock polarity */
      uint16_t _MSTR : 1;      /*< 1: SPI device in master mode */
      uint16_t _BAUD_RATE : 3; /*< Spi baud rate setting. Check SPI_CTRL_BR_FPCLK macros */
      uint16_t _SPI_EN : 1;    /*< 1: Enable SPI device */
      uint16_t _LSBFIRST : 1;  /*< 1: send LSB first, 0: send MSB first */
      uint16_t _NSS_LVL : 1;   /*< NSS pin logic level; 1: high 0: low. Use when _NSS_CFG == 1 */
      uint16_t _NSS_CFG : 1;   /*< NSS pin control configuration; 1: SW NSS control, 0: HW NSS control */
      uint16_t _RXONLY : 1;    /*< Rx only (simplex) in 2-wire mode (use when _SDDMEN==0) */
      uint16_t _DFF : 1;       /*< Data frame length: 1: 16bit, 0: 8bit */
      uint16_t _CRCNEXT : 1;   /*< I have no idea. Check RM if you're interested in knowing */
      uint16_t _CRCEN : 1;     /*< Enable HW CRC check */
      uint16_t _SDDM_TX : 1;   /*< Enable TX only in SDDM. Use in conjucture with _SDDMEN" */
      uint16_t _SDDMEN : 1;    /*< Single direction data-mode enable (BIDIMODE in Reference manual). */
    };
    uint16_t data;
  } CTRL1;

  /* SPI_CTRL2 */
  union CTRL2
  {
    struct
    {
      uint8_t _RXDMAEN : 1;   /*< RX DMA enable */
      uint8_t _TXDMAEN : 1;   /*< TX DMA enable */
      uint8_t _NSSOE : 1;     /*< NSS output enable. You don't need to touch this bit if _NSS_CFG==1. I think :) */
      uint8_t _reserved2 : 2; /*< don't care */
      uint8_t _ERRIE : 1;     /*< Error interrupt enable. */
      uint8_t _RXNEIE : 1;    /*< RX buffer not empty interrupt enable */
      uint8_t _TXEIE : 1;     /*< TX buffer empty interrupt enable */
    };
    uint8_t data; /*< upper 16 bits contain SPI_CTRL1 register. Lower 16 bits contain SPI_CTRL2 register */
  } CTRL2;
} SPI_Config;

/**
 * @name SPI_CTRL1_BR_(2|4|8|16|32|64|128|256)
 *
 * @brief Baud rate setting domain, this register cannot be changed after SPI device
 *        is enabled.
 */
#define SPI_CTRL1_BR_FPCLK_2 ((SPI_ConfigCtrl1)0))
#define SPI_CTRL1_BR_FPCLK_4   ((SPI_ConfigCtrl1)(0b001))
#define SPI_CTRL1_BR_FPCLK_8   ((SPI_ConfigCtrl1)(0b010))
#define SPI_CTRL1_BR_FPCLK_16  ((SPI_ConfigCtrl1)(0b011))
#define SPI_CTRL1_BR_FPCLK_32  ((SPI_ConfigCtrl1)(0b100))
#define SPI_CTRL1_BR_FPCLK_64  ((SPI_ConfigCtrl1)(0b101))
#define SPI_CTRL1_BR_FPCLK_128 ((SPI_ConfigCtrl1)(0b110))
#define SPI_CTRL1_BR_FPCLK_256 ((SPI_ConfigCtrl1)(0b111))

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
  SPI_Err_ConfigFail,
  SPI_Err_AlreadyEnabled,
  SPI_Err_NotEnabled,
  SPI_Err_NoSuchDevice,
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
SPI_Err spi_configure_device(SPI_Device dev, SPI_Config config);

/**
 * @fn spi_write
 *
 * @brief Write data to selected SPI device. If SPI packet size is 8bits, than only lower byte will be written to SPI device.
 *
 * @param dev Desired SPI device
 *
 * @param data Data to write
 */
void spi_write(SPI_Device dev, uint16_t data);

/**
 * @fn spi_read
 *
 * @brief Read data from SPI device.
 *
 * @param dev Desired SPI device
 *
 * @return data Data that was read from the SPI device. If max data packet on SPI device is 8 bits, than lower byte contains
 * received data
 */
uint16_t spi_read(SPI_Device dev);

/**
 * @fn spi_is_enabled
 *
 * @brief
 *
 * @param dev
 *
 * @return is_enabled
 */
bool spi_is_enabled(SPI_Device dev);

/**
 * @fn spi_is_configured
 *
 * @brief
 *
 * @param dev
 *
 * @return is_configured
 */
bool spi_is_configured(SPI_Device dev);

/**
 * @fn spi_packet_size
 *
 * @brief
 *
 * @param dev
 *
 * @return packet size
 */
uint8_t spi_packet_size(SPI_Device dev);

/**
 * @fn spi_wait_tx
 *
 * @brief Wait for TX to finish.
 *
 * @param dev
 *
 */
void spi_wait_tx(SPI_Device dev);

#endif
