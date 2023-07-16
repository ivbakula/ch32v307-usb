#ifndef INC_RCC_DEVIDS_H
#define INC_RCC_DEVIDS_H

#include "helpers.h"

typedef uint32_t RCCId;

/* BITS 31:30 are used as device mask. */
#define RCC_AHB_DEVICE_MASK  ((RCCId)0)
#define RCC_APB1_DEVICE_MASK ((RCCId)U32_BIT(30))
#define RCC_APB2_DEVICE_MASK ((RCCId)U32_BIT(31))
#define RCC_DEVICE_EN_MASK   ((RCCId)(U32_BIT(30) | U32_BIT(31))

/* RCC Device ID definitions */
#define RCC_DMA1_EN ((RCCId)(U32_BIT(0) | RCC_AHB_DEVICE_MASK))
#define RCC_DMA2_EN ((RCCId)(U32_BIT(1) | RCC_AHB_DEVICE_MASK))
#define RCC_SRAMEN  ((RCCId)(U32_BIT(2) | RCC_AHB_DEVICE_MASK))
#define RCC_CRCEN   ((RCCId)(U32_BIT(6) | RCC_AHB_DEVICE_MASK))
#define RCC_FSMCEN  ((RCCId)(U32_BIT(8) | RCC_AHB_DEVICE_MASK))
#define RCC_RNGEN   ((RCCId)(U32_BIT(9) | RCC_AHB_DEVICE_MASK))
#define RCC_SDIOEN  ((RCCId)(U32_BIT(10) | RCC_AHB_DEVICE_MASK))
#define RCC_USBHSEN ((RCCId)(U32_BIT(11) | RCC_AHB_DEVICE_MASK))
#define RCC_OTGFSEN ((RCCId)(U32_BIT(12) | RCC_AHB_DEVICE_MASK))
#define RCC_DVPEN   ((RCCId)(U32_BIT(13) | RCC_AHB_DEVICE_MASK))
#define RCC_ETHMACEN ((RCCId)(U32_BIT(14) | RCC_AHB_DEVICE_MASK))
#define RCC_ETHMACTXEN ((RCCId)(U32_BIT(15) | RCC_AHB_DEVICE_MASK))
#define RCC_ETHMACRXEN ((RCCId)(U32_BIT(16) | RCC_AHB_DEVICE_MASK))
#define RCC_BLECEN     ((RCCId)(U32_BIT(16) | RCC_AHB_DEVICE_MASK))
#define RCC_BLESEN     ((RCCId)(U32_BIT(17) | RCC_AHB_DEVICE_MASK))

#define RCC_AFIOEN ((RCCId)(U32_BIT(0) | RCC_APB2_DEVICE_MASK))
#define RCC_IOPAEN ((RCCId)(U32_BIT(2) | RCC_APB2_DEVICE_MASK))
#define RCC_IOPBEN ((RCCId)(U32_BIT(3) | RCC_APB2_DEVICE_MASK))
#define RCC_IOPCEN ((RCCId)(U32_BIT(4) | RCC_APB2_DEVICE_MASK))
#define RCC_IOPDEN ((RCCId)(U32_BIT(5) | RCC_APB2_DEVICE_MASK))
#define RCC_IOPEEN ((RCCId)(U32_BIT(6) | RCC_APB2_DEVICE_MASK))
#define RCC_ADC1EN ((RCCId)(U32_BIT(9) | RCC_APB2_DEVICE_MASK))
#define RCC_ADC2EN ((RCCId)(U32_BIT(10) | RCC_APB2_DEVICE_MASK))
#define RCC_TIM1EN ((RCCId)(U32_BIT(11) | RCC_APB2_DEVICE_MASK))
#define RCC_SPI1EN ((RCCId)(U32_BIT(12) | RCC_APB2_DEVICE_MASK))
#define RCC_TIM8EN ((RCCId)(U32_BIT(13) | RCC_APB2_DEVICE_MASK))
#define RCC_USART1EN ((RCCId)(U32_BIT(14) | RCC_APB2_DEVICE_MASK))
#define RCC_TIM9EN   ((RCCId)(U32_BIT(19) | RCC_APB2_DEVICE_MASK))
#define RCC_TIM10EN  ((RCCId)(U32_BIT(20) | RCC_APB2_DEVICE_MASK))

#define RCC_TIM2EN ((RCCId)(U32_BIT(0) | RCC_APB1_DEVICE_MASK))
#define RCC_TIM3EN ((RCCId)(U32_BIT(1) | RCC_APB1_DEVICE_MASK))
#define RCC_TIM4EN ((RCCId)(U32_BIT(2) | RCC_APB1_DEVICE_MASK))
#define RCC_TIM5EN ((RCCId)(U32_BIT(3) | RCC_APB1_DEVICE_MASK))
#define RCC_TIM6EN ((RCCId)(U32_BIT(4) | RCC_APB1_DEVICE_MASK))
#define RCC_TIM7EN ((RCCId)(U32_BIT(5) | RCC_APB1_DEVICE_MASK))
#define RCC_UART6EN ((RCCId)(U32_BIT(6) | RCC_APB1_DEVICE_MASK))
#define RCC_UART7EN ((RCCId)(U32_BIT(7) | RCC_APB1_DEVICE_MASK))
#define RCC_UART8EN ((RCCId)(U32_BIT(8) | RCC_APB1_DEVICE_MASK))
#define RCC_WWDGEN  ((RCCId)(U32_BIT(11) | RCC_APB1_DEVICE_MASK))
#define RCC_SPI2EN  ((RCCId)(U32_BIT(14) | RCC_APB1_DEVICE_MASK))
#define RCC_SPI3EN  ((RCCId)(U32_BIT(15) | RCC_APB1_DEVICE_MASK))
#define RCC_USART2EN ((RCCId)(U32_BIT(17) | RCC_APB1_DEVICE_MASK))
#define RCC_USART3EN ((RCCId)(U32_BIT(18) | RCC_APB1_DEVICE_MASK))
#define RCC_UART4EN  ((RCCId)(U32_BIT(19) | RCC_APB1_DEVICE_MASK))
#define RCC_UART5EN  ((RCCId)(U32_BIT(20) | RCC_APB1_DEVICE_MASK))
#define RCC_I2C1EN   ((RCCId)(U32_BIT(21) | RCC_APB1_DEVICE_MASK))
#define RCC_I2C2EN   ((RCCId)(U32_BIT(22) | RCC_APB1_DEVICE_MASK))
#define RCC_USBDEN   ((RCCId)(U32_BIT(23) | RCC_APB1_DEVICE_MASK))
#define RCC_CAN1EN   ((RCCId)(U32_BIT(25) | RCC_APB1_DEVICE_MASK))
#define RCC_CAN2EN   ((RCCId)(U32_BIT(26) | RCC_APB1_DEVICE_MASK))
#define RCC_BKPEN    ((RCCId)(U32_BIT(27) | RCC_APB1_DEVICE_MASK))
#define RCC_PWREN    ((RCCId)(U32_BIT(28) | RCC_APB1_DEVICE_MASK))
#define RCC_DACEN    ((RCCId)(U32_BIT(29) | RCC_APB1_DEVICE_MASK))

#endif
