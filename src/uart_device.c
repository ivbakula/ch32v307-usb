#include "uart_device.h"

#ifdef UNITTEST
UART_Regfile uart1_regfile;
UART_Regfile uart2_regfile;
UART_Regfile uart3_regfile;
UART_Regfile uart4_regfile;
UART_Regfile uart5_regfile;
UART_Regfile uart6_regfile;
UART_Regfile uart7_regfile;
UART_Regfile uart8_regfile;
#endif

UART_Err uart_enable_device(UART_Device dev, UART_PinConfig pin_config)
{
  return UART_Err_Success;
}

UART_Err uart_disable_device(UART_Device dev)
{
  return UART_Err_Success;
}

UART_Err uart_reset_device(UART_Device dev)
{
  return UART_Err_Success;
}

UART_Err uart_configure_device(UART_Device dev, uint32_t baud, uint32_t apbclk)
{
  return UART_Err_Success;
}

void uart_putc(UART_Device dev, char c)
{
}

char uart_getc(UART_Device dev)
{
  return 0;
}
