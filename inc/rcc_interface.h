#ifndef INC_RCC_INTERFACE_H
#define INC_RCC_INTERFACE_H

#include <stdbool.h>

#include "helpers.h"

typedef uint32_t RCC_DevId;

#define RCC_ENABLE  true
#define RCC_DISABLE false

/**
 * @fn rcc_pcendis
 *
 * @brief Enable or disable peripherial device clock.
 * @param device: RCC device id of desired device. RCC device IDs are defined below, in section "RCC_DevID_definitions"
 * @param on: [true] Enable peripherial clock for given device
 *            [false] Disable peripherial clock for given device
 */
void rcc_pcendis(RCC_DevId devid, bool on);

/**
 * @name RCC_DevID_definitions
 * @brief In this section RCC_DevID numbers are defined. These numers are
 *        standard unsigned 32bit integers and RCC_DevID structure is explained below.
 *
 *        In ch32v307 MCU, peripherials are distributed along 3 banks: AHB, APB2 and APB2.
 *        Each of these banks has it's own register for enabling/disabling clock to
 *        peripherial that is connected to it. So APB has RCC_AHBPCENR, APB1 has
 *        RCC_APB1PCENR and APB2 has RCC_APB2PCENR.
 *
 *        These macros should provide uniform way of enabling a device with single function, because
 *        they encode informations about BANK and device in single uint32.
 *
 *        The structure of RCC_DevID is shown in following ilustration:
 *
 *        31 30 29 ...  0
 *       +--+--+--+...+--+
 *       |  |  |  |   |  |
 *       +--+--+--+...+--+
 *
 *       |-----|---------|
 *        MASK  DeviceBit
 *
 *       As you can see from this picture, bits 31 and 30 are used as mask which will
 *       designate bank on which device is connected to, and bits 0-29 are used as device designator.
 *
 *       NOTE: [31:30] bitmask is called RCC_PERIPH_BANK_MASK
 *
 *       NOTE that DeviceBit in RCC_DevId corresponds to device bit in RCC_(AHBPCENR|APB1PCENR|APB2PCENR)
 *       register
 */
///@{
/* BITS 31:30 are used as device mask. */
#define RCC_AHB_DEVICE_MASK  ((RCC_DevId)0)
#define RCC_APB1_DEVICE_MASK ((RCC_DevId)0x40000000)
#define RCC_APB2_DEVICE_MASK ((RCC_DevId)0x80000000)
#define RCC_PERIPH_BANK_MASK ((RCC_DevId)0xc0000000)

/* RCC Device ID definitions */
#define RCC_DMA1_EN    ((RCC_DevId)(U32_BIT(0) | RCC_AHB_DEVICE_MASK))
#define RCC_DMA2_EN    ((RCC_DevId)(U32_BIT(1) | RCC_AHB_DEVICE_MASK))
#define RCC_SRAMEN     ((RCC_DevId)(U32_BIT(2) | RCC_AHB_DEVICE_MASK))
#define RCC_CRCEN      ((RCC_DevId)(U32_BIT(6) | RCC_AHB_DEVICE_MASK))
#define RCC_FSMCEN     ((RCC_DevId)(U32_BIT(8) | RCC_AHB_DEVICE_MASK))
#define RCC_RNGEN      ((RCC_DevId)(U32_BIT(9) | RCC_AHB_DEVICE_MASK))
#define RCC_SDIOEN     ((RCC_DevId)(U32_BIT(10) | RCC_AHB_DEVICE_MASK))
#define RCC_USBHSEN    ((RCC_DevId)(U32_BIT(11) | RCC_AHB_DEVICE_MASK))
#define RCC_OTGFSEN    ((RCC_DevId)(U32_BIT(12) | RCC_AHB_DEVICE_MASK))
#define RCC_DVPEN      ((RCC_DevId)(U32_BIT(13) | RCC_AHB_DEVICE_MASK))
#define RCC_ETHMACEN   ((RCC_DevId)(U32_BIT(14) | RCC_AHB_DEVICE_MASK))
#define RCC_ETHMACTXEN ((RCC_DevId)(U32_BIT(15) | RCC_AHB_DEVICE_MASK))
#define RCC_ETHMACRXEN ((RCC_DevId)(U32_BIT(16) | RCC_AHB_DEVICE_MASK))
#define RCC_BLECEN     ((RCC_DevId)(U32_BIT(16) | RCC_AHB_DEVICE_MASK))
#define RCC_BLESEN     ((RCC_DevId)(U32_BIT(17) | RCC_AHB_DEVICE_MASK))

#define RCC_AFIOEN   ((RCC_DevId)(U32_BIT(0) | RCC_APB2_DEVICE_MASK))
#define RCC_IOPAEN   ((RCC_DevId)(U32_BIT(2) | RCC_APB2_DEVICE_MASK))
#define RCC_IOPBEN   ((RCC_DevId)(U32_BIT(3) | RCC_APB2_DEVICE_MASK))
#define RCC_IOPCEN   ((RCC_DevId)(U32_BIT(4) | RCC_APB2_DEVICE_MASK))
#define RCC_IOPDEN   ((RCC_DevId)(U32_BIT(5) | RCC_APB2_DEVICE_MASK))
#define RCC_IOPEEN   ((RCC_DevId)(U32_BIT(6) | RCC_APB2_DEVICE_MASK))
#define RCC_ADC1EN   ((RCC_DevId)(U32_BIT(9) | RCC_APB2_DEVICE_MASK))
#define RCC_ADC2EN   ((RCC_DevId)(U32_BIT(10) | RCC_APB2_DEVICE_MASK))
#define RCC_TIM1EN   ((RCC_DevId)(U32_BIT(11) | RCC_APB2_DEVICE_MASK))
#define RCC_SPI1EN   ((RCC_DevId)(U32_BIT(12) | RCC_APB2_DEVICE_MASK))
#define RCC_TIM8EN   ((RCC_DevId)(U32_BIT(13) | RCC_APB2_DEVICE_MASK))
#define RCC_USART1EN ((RCC_DevId)(U32_BIT(14) | RCC_APB2_DEVICE_MASK))
#define RCC_TIM9EN   ((RCC_DevId)(U32_BIT(19) | RCC_APB2_DEVICE_MASK))
#define RCC_TIM10EN  ((RCC_DevId)(U32_BIT(20) | RCC_APB2_DEVICE_MASK))

#define RCC_TIM2EN   ((RCC_DevId)(U32_BIT(0) | RCC_APB1_DEVICE_MASK))
#define RCC_TIM3EN   ((RCC_DevId)(U32_BIT(1) | RCC_APB1_DEVICE_MASK))
#define RCC_TIM4EN   ((RCC_DevId)(U32_BIT(2) | RCC_APB1_DEVICE_MASK))
#define RCC_TIM5EN   ((RCC_DevId)(U32_BIT(3) | RCC_APB1_DEVICE_MASK))
#define RCC_TIM6EN   ((RCC_DevId)(U32_BIT(4) | RCC_APB1_DEVICE_MASK))
#define RCC_TIM7EN   ((RCC_DevId)(U32_BIT(5) | RCC_APB1_DEVICE_MASK))
#define RCC_UART6EN  ((RCC_DevId)(U32_BIT(6) | RCC_APB1_DEVICE_MASK))
#define RCC_UART7EN  ((RCC_DevId)(U32_BIT(7) | RCC_APB1_DEVICE_MASK))
#define RCC_UART8EN  ((RCC_DevId)(U32_BIT(8) | RCC_APB1_DEVICE_MASK))
#define RCC_WWDGEN   ((RCC_DevId)(U32_BIT(11) | RCC_APB1_DEVICE_MASK))
#define RCC_SPI2EN   ((RCC_DevId)(U32_BIT(14) | RCC_APB1_DEVICE_MASK))
#define RCC_SPI3EN   ((RCC_DevId)(U32_BIT(15) | RCC_APB1_DEVICE_MASK))
#define RCC_USART2EN ((RCC_DevId)(U32_BIT(17) | RCC_APB1_DEVICE_MASK))
#define RCC_USART3EN ((RCC_DevId)(U32_BIT(18) | RCC_APB1_DEVICE_MASK))
#define RCC_UART4EN  ((RCC_DevId)(U32_BIT(19) | RCC_APB1_DEVICE_MASK))
#define RCC_UART5EN  ((RCC_DevId)(U32_BIT(20) | RCC_APB1_DEVICE_MASK))
#define RCC_I2C1EN   ((RCC_DevId)(U32_BIT(21) | RCC_APB1_DEVICE_MASK))
#define RCC_I2C2EN   ((RCC_DevId)(U32_BIT(22) | RCC_APB1_DEVICE_MASK))
#define RCC_USBDEN   ((RCC_DevId)(U32_BIT(23) | RCC_APB1_DEVICE_MASK))
#define RCC_CAN1EN   ((RCC_DevId)(U32_BIT(25) | RCC_APB1_DEVICE_MASK))
#define RCC_CAN2EN   ((RCC_DevId)(U32_BIT(26) | RCC_APB1_DEVICE_MASK))
#define RCC_BKPEN    ((RCC_DevId)(U32_BIT(27) | RCC_APB1_DEVICE_MASK))
#define RCC_PWREN    ((RCC_DevId)(U32_BIT(28) | RCC_APB1_DEVICE_MASK))
#define RCC_DACEN    ((RCC_DevId)(U32_BIT(29) | RCC_APB1_DEVICE_MASK))
///@}

#endif
