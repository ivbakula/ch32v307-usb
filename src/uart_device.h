#ifndef SRC_UART_DEVICE_H
#define SRC_UART_DEVICE_H
#include <stdint.h>
#include <stddef.h>
#include "uart_interface.h"
#include "rcc_interface.h"

#define USART_STATR_OFFSET ((uintptr_t)0x00)
#define USART_DATAR_OFFSET ((uintptr_t)0x04)
#define USART_BRR_OFFSET   ((uintptr_t)0x08)
#define USART_CTRL1_OFFSET ((uintptr_t)0x0c)
#define USART_CTRL2_OFFSET ((uintptr_t)0x10)
#define USART_CTRL3_OFFSET ((uintptr_t)0x14)
#define USART_GPR_OFFSET   ((uintptr_t)0x18)

#define USART1_BASE ((uintptr_t)0x40013800)
#define USART2_BASE ((uintptr_t)0x40004400)
#define USART3_BASE ((uintptr_t)0x40004800)
#define UART4_BASE ((uintptr_t)0x40004C00)
#define UART5_BASE ((uintptr_t)0x40005000)
#define UART6_BASE ((uintptr_t)0x40001800)
#define UART7_BASE ((uintptr_t)0x40001C00)
#define UART8_BASE ((uintptr_t)0x40002000)

const uintptr_t __usart_mmio_mapping[] = {
  USART1_BASE,
  USART2_BASE,
  USART3_BASE,
  UART4_BASE,
  UART5_BASE,
  UART6_BASE,
  UART7_BASE,
  UART8_BASE,
};

const uintptr_t __usart_rcc_mapping[] = {
  RCC_USART1EN,
  RCC_USART2EN,
  RCC_USART3EN,
  RCC_UART4EN,
  RCC_UART5EN,
  RCC_UART6EN,
  RCC_UART7EN,
  RCC_UART8EN,
};

#endif
