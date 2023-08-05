#include <unity.h>
#include "uart_interface.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_uart_enable_disable(void)
{
}

void test_uart_configure_device(void)
{
}

void test_uart_reset_device(void)
{
}

void test_uart_putc(void)
{
}

void test_uart_getc(void)
{
}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_uart_enable_disable);
  RUN_TEST(test_uart_configure_device);
  RUN_TEST(test_uart_reset_device);
  RUN_TEST(test_uart_putc);
  RUN_TEST(test_uart_getc);
  return UNITY_END();
}
