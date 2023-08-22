#ifndef INC_DVP_INTERFACE_H
#define INC_DVP_INTERFACE_H

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
  DVP_Err_Success,
  DVP_Err_ConfigFail,
  DVP_Err_AlreadyEnabled,
  DVP_Err_AlreadyConfigured,
  DVP_Err_NotEnabled,
  DVP_Err_NoSuchDevice,
  DVP_Err_DeviceOutOfRange,
} DVP_Err;

typedef enum
{
  DVP_Device1,
} DVP_Device;

typedef enum
{
  DVP_BusWidth_8bit = 0b00,
  DVP_BusWidth_10bit = 0b01,
  DVP_BusWidth_12bit = 0b10,
} DVP_BusWidth;

#define DVP_VSYNC_POL_LOW  0
#define DVP_VSYNC_POL_HIGH 1

#define DVP_HSYNC_POL_LOW  1
#define DVP_HSYNC_POL_HIGH 0

#define DVP_PCLK_POL_POSEDGE 0
#define DVP_PCLK_POL_NEGEDGE 1

#define DVP_DATA_FORMAT_RAW  0
#define DVP_DATA_FORMAT_JPEG 1

#define DVP_ENABLE_DMA  1
#define DVP_DISABLE_DMA 0

#define DVP_CAPTURE_MODE_SNAPSHOT   1
#define DVP_CAPTURE_MODE_CONTINUOUS 0

#define DVP_CROP_DISABLED 0
#define DVP_CROP_ENABLED  1

#define DVP_FCRC_ALL_FRAMES        (0b00)
#define DVP_FCRC_EVERY_OTHER_FRAME (0b01)
#define DVP_FCRC_EVERY_THIRD_FRAME (0b10)
#define DVP_FCRC_RESERVED          (0b11)

#define DVP_FRAME_START_INTERRUPT_EN  1
#define DVP_FRAME_START_INTERRUPT_DIS 0
#define DVP_ROW_DONE_INTERRUPT_EN     1
#define DVP_ROW_DONE_INTERRUPT_DIS    0
#define DVP_FRM_DONE_INTERRUPT_EN     1
#define DVP_FRM_DONE_INTERRUPT_DIS    0
#define DVP_FIFO_OV_INTERRUPT_EN      1
#define DVP_FIFO_OV_INTERRUPT_DIS     0
#define DVP_STP_FRM_INTERRUPT_EN      1
#define DVP_STP_FRM_INTERRUPT_DIS     0

/**
 * @brief This structure contains register bit fields that are necessary at device configuration
 */
typedef struct
{
  /* CR0 register */
  union
  {
    struct
    {
      uint8_t DVP_ENABLE : 1;     /*< Enable DVP device */
      uint8_t DVP_VSYNC_POL : 1;  /*< VSYNC signal polarity 1: high VSYNC. 0: low VSYNC*/
      uint8_t DVP_HSYNC_POL : 1;  /*< HSYNC signal polarity 1: low HSYNC. 0: high HSYNC */
      uint8_t DVP_PCLK_POL : 1;   /*< Pixel clock polarity configuration. 1: falling edge; 0: rising edge */
      uint8_t DVP_DATA_WIDTH : 2; /*< DVP data bus width. Check DVP_BusWidth enum*/
      uint8_t DVP_MODE : 1;       /*< 0: raw mode; 1: JPEG mode */
      uint8_t reserved0 : 1;      /*< Don't care */
    };
    uint8_t data;
  } CR0;

  /* CR1 register */
  union
  {
    struct
    {
      uint8_t DVP_DMA_ENABLE : 1;   /*< Enable DMA for DVP device */
      uint8_t DVP_ALL_CLR : 1;      /*< Flag and FIFO clr ctrl; */
      uint8_t DVP_RCV_CLR : 1;      /*< Receive logic reset control */
      uint8_t DVP_BUF_TOG : 1;      /*< Buffer address flag */
      uint8_t DVP_CAPTURE_MODE : 1; /*< 0: Continuous mode; 1: Snapshot mode */
      uint8_t DVP_CROP : 1;         /*< Crop function control. */
      uint8_t DVP_FCRC : 2;         /*< DVP frame capture rate control */
    };
    uint8_t data;
  } CR1;

  /* IER register (Interrupt enable register) */
  union
  {
    struct
    {
      uint8_t DVP_FRM_STRT : 1; /*< Frame start interrupt enable */
      uint8_t DVP_ROW_DONE : 1; /*< Row done interrupt enable. */
      uint8_t DVP_FRM_DONE : 1; /*< Frame done interrupt enable. */
      uint8_t DVP_FIFO_OV : 1;  /*< RCV FIFO buffer overflow interrupt enable. */
      uint8_t DVP_STP_FRM : 1;  /*< Frame stop interrupt enable. */
      uint8_t reserved0 : 3;    /*< Don't care */
    };
    uint8_t data;
  } IER;

  uint16_t DVP_ROW_NUM;   /*< Number of Rows contained in a frame of image data. */
  uint16_t DVP_COL_NUM;   /*< Number pixels in a row (number of PCLK cycles per row) */
  uintptr_t DVP_DMA_BUF0; /*< Address of DMA buffer 1 */
  uintptr_t DVP_DMA_BUF1; /*< Address of DMA buffer 2 */
} DVP_Config;

typedef uint8_t DVP_PinConfig;

/**
 * @name DVP_DEFAULT_MAPPING
 *
 * @brief Default PIN mapping for DVP device.
 *
 * |------+-------+-------+------+-----+------+-----+-----+------+-----+-----+-----+------+------+-----+-----|
 * | FUNC | HSYNC | VSYNC | PCLK | D0  | D1   | D2  | D3  | D4   | D5  | D6  | D7  | D8   | D9   | D10 | D11 |
 * |------+-------+-------+------+-----+------+-----+-----+------+-----+-----+-----+------+------+-----+-----|
 * | PIN  | PA4   | PA5   | PA6  | PA9 | PA10 | PC8 | PC9 | PC11 | PB6 | PB8 | PB9 | PC10 | PC12 | PD6 | PD2 |
 * |------+-------+-------+------+-----+------+-----+-----+------+-----+-----+-----+------+------+-----+-----|
 *
 */
#define DVP_DEFAULT_MAPPING ((DVP_PinConfig)0b00)

/**
 * @name DVP_ALTERNATE_MAPPING_1
 *
 * @brief Alternate PIN mapping for DVP device.
 *
 * |------+-------+-------+------+-----+------+-----+-----+------+-----+-----+-----+------+------+-----+-----|
 * | FUNC | HSYNC | VSYNC | PCLK | D0  | D1   | D2  | D3  | D4   | D5  | D6  | D7  | D8   | D9   | D10 | D11 |
 * |------+-------+-------+------+-----+------+-----+-----+------+-----+-----+-----+------+------+-----+-----|
 * | PIN  | PA4   | PA5   | PA6  | PA9 | PA10 | PC8 | PC9 | PC11 | PB3 | PB8 | PB9 | PC10 | PC12 | PD6 | PD2 |
 * |------+-------+-------+------+-----+------+-----+-----+------+-----+-----+-----+------+------+-----+-----|
 *
 */
#define DVP_ALTERNATE_MAPPING_1 ((DVP_PinConfig)0b01)

/**
 * @fn dvp_enable_device
 *
 * @brief
 *
 * @param dev
 *
 * @param pin_config
 *
 * @return DVP_Err
 */
DVP_Err dvp_enable_device(DVP_Device dev, DVP_PinConfig pin_config);

/**
 * @fn dvp_disable_device
 *
 * @brief
 *
 * @param dev
 *
 * @return
 */
DVP_Err dvp_disable_device(DVP_Device dev);

/**
 * @fn dvp_reset_device
 *
 * @brief
 *
 * @param dev
 *
 * @retuirn DVP_Err
 */
DVP_Err dvp_reset_device(DVP_Device dev);

/**
 * @fn dvp_configure_device
 *
 * @brief
 *
 * @param dev
 *
 * @param config
 */
DVP_Err dvp_configure_device(DVP_Device dev, DVP_Config config);

/**
 * @fn dvp_is_enabled
 *
 * @brief
 *
 * @param dev
 *
 * @return DVP_Err DVP_AlreadyEnabled if device is enabled , Appropriate error code otherwise.
 */
DVP_Err dvp_is_enabled(DVP_Device dev);

/**
 * @fn dvp_is_configured
 *
 * @brief
 *
 * @param dev
 *
 * @return DVP_Err DVP_AlreadyConfigured if device is configured, Appropriate Error code otherwise.
 */
DVP_Err dvp_is_configured(DVP_Device dev);

/**
 * @fn dvp_dump_registers
 *
 * @brief
 *
 * @param dev
 *
 */
void dvp_dump_registers(DVP_Device dev);

#endif
