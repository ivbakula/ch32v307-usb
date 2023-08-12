#include <pthread.h>
#include <unistd.h>
#include <unity.h>

#include "gpio_device.h"
#include "rcc_device.h"
#include "uart_device.h"

void setUp(void)
{
}

void tearDown(void)
{
  memset(&uart1_regfile, 0, sizeof(uart1_regfile));
}

void test_uart_enable_disable(void)
{
  const uint32_t push_pull_alternate_output_50MHz_PA9 = (0b1011 << 4);
  const uint32_t pull_up_input_PA10 = (0b1000 << 8);
  const uint32_t pa9_mask = (0xf << 4);
  const uint32_t pa10_mask = (0xf << 8);

  /* Lock TX pin and fail with ConfigFail error code */
  gpio_lock_pin(RCC_USART1EN, PA9);
  TEST_ASSERT_EQUAL(UART_Err_ConfigFail, uart_enable_device(UART_Device1, UART1_DEFAULT_MAPPING));
  gpio_unlock_pin(RCC_USART1EN, PA9);

  /* Unlock TX pin and successfuly enable UART */
  TEST_ASSERT_EQUAL(UART_Err_Success, uart_enable_device(UART_Device1, UART1_DEFAULT_MAPPING));
  TEST_ASSERT_EQUAL(U32_BIT(14) | U32_BIT(2), rcc_regfile.R32_RCC_APB2PCENR);
  TEST_ASSERT_EQUAL(push_pull_alternate_output_50MHz_PA9, gpioa_regfile.R32_GPIO_CFGHR & pa9_mask);
  TEST_ASSERT_EQUAL(pull_up_input_PA10, gpioa_regfile.R32_GPIO_CFGHR & pa10_mask);

  /* Fail with AlreadyEnabled error code  */
  TEST_ASSERT_EQUAL(UART_Err_AlreadyEnabled, uart_enable_device(UART_Device1, UART1_DEFAULT_MAPPING));

  /* Disable successfuly */
  TEST_ASSERT_EQUAL(UART_Err_Success, uart_disable_device(UART_Device1));
  TEST_ASSERT_EQUAL(U32_BIT(2), rcc_regfile.R32_RCC_APB2PCENR); /* Only GPIOA port is enabled */

  /* Fail with NotEnabled error code */
  TEST_ASSERT_EQUAL(UART_Err_NotEnabled, uart_disable_device(UART_Device1));
}

void test_uart_configure_device(void)
{
  const uint32_t baud = 9600;
  const uint32_t apbclk = 48000000;
  /* Fail with NotEnabled error code */
  TEST_ASSERT_EQUAL(UART_Err_NotEnabled, uart_configure_device(UART_Device1, baud, apbclk));

  /* Pass */
  TEST_ASSERT_EQUAL(UART_Err_Success, uart_enable_device(UART_Device1, UART1_DEFAULT_MAPPING));
  TEST_ASSERT_EQUAL(UART_Err_Success, uart_configure_device(UART_Device1, baud, apbclk));

  // TODO check if register values for given baud rate and apbclk are correct
}

void test_uart_reset_device(void)
{
  // TODO as it will not be implemented in first drop
}

/*
 * This function will simulate real process of writing to OUTDR  register; It will
 * force uart_putc function to block until "UART device"" is ready to write
 */
void *test_uart_putc_helper(void *)
{
  sleep(1);
  uart1_regfile.R32_UART_STATR |= U32_BIT(6);
  return NULL;
}

void test_uart_putc(void)
{
  pthread_t helper_thread;
  pthread_create(&helper_thread, NULL, test_uart_putc_helper, NULL);
  uart_putc(UART_Device1, 'h');

  TEST_ASSERT_EQUAL('h', uart1_regfile.R32_UART_DATAR);
  pthread_join(helper_thread, NULL);
}

void *test_uart_getc_helper(void *)
{
  sleep(1);
  uart1_regfile.R32_UART_DATAR = 'H';
  uart1_regfile.R32_UART_STATR |= U32_BIT(5);
  return NULL;
}

void test_uart_getc(void)
{
  pthread_t helper_thread;
  pthread_create(&helper_thread, NULL, test_uart_getc_helper, NULL);

  TEST_ASSERT_EQUAL('H', uart_getc(UART_Device1));
  pthread_join(helper_thread, NULL);
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
