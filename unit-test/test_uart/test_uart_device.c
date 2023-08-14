#include <unity.h>

#include <stdint.h>

#include "uart_device.h"
#include "Mockmmio_ops.h"
#include "Mockrcc_interface.h"
#include "Mockgpio_interface.h"

// clang-format off
const unsigned char uart_instantiated[8] = { 1, 0, 0, 0, 0, 0, 0, 0 };
UART_Instance UART_Instances[8] = {
  {
    .base = UART1_BASE,
    .rcc_devid = RCC_USART1EN,
    .irqn = USART1_IRQn,
    .enabled = false,
    .configured = false,
    .chosen_pinconfig = UART1_DEFAULT_MAPPING,
    .no_pin_configs = 4,
    .pin_configuration = {
      { PA8,  PA9,  PA10, PA11, PA12 },
      { PA8,  PB6,  PB7,  PA11, PA12 },
      { PA10, PB15, PA8,  PA5,  PA9  },
      { PA5,  PA6,  PA7,  PC4,  PC5  }
    }
  },
};
// clang-format on

void setUp(void)
{
  UART_Instances[0].base = UART1_BASE;
  UART_Instances[0].rcc_devid = RCC_USART1EN;
  UART_Instances[0].irqn = USART1_IRQn;
  UART_Instances[0].enabled = false;
  UART_Instances[0].configured = false;
  UART_Instances[0].chosen_pinconfig = UART1_DEFAULT_MAPPING;
  UART_Instances[0].no_pin_configs = 4;
  // clang-format off
  GPIO_Pin pin_configuration[4][5] = {
      { PA8,  PA9,  PA10, PA11, PA12 },
      { PA8,  PB6,  PB7,  PA11, PA12 },
      { PA10, PB15, PA8,  PA5,  PA9  },
      { PA5,  PA6,  PA7,  PC4,  PC5  }
  };
  // clang-format on

  memcpy(&UART_Instances[0].pin_configuration[0][0], &pin_configuration[0][0], sizeof(pin_configuration));  
}

void tearDown(void)
{
  memset(&UART_Instances[0], 0, sizeof(UART_Instances));
}

void test_uart_enable_success(void)
{
  const uint32_t push_pull_alternate_output_50MHz_PA9 = (0b1011 << 4);
  const uint32_t pull_up_input_PA10 = (0b1000 << 8);
  const uint32_t pa9_mask = (0xf << 4);
  const uint32_t pa10_mask = (0xf << 8);

  rcc_pcendis_Expect(RCC_USART1EN, RCC_ENABLE);

  gpio_port_enable_Expect(GET_GPIO_PORT(PA9));
  gpio_port_enable_Expect(GET_GPIO_PORT(PA10));

  gpio_lock_pin_ExpectAndReturn(RCC_USART1EN, PA9, GPIO_Success);
  gpio_lock_pin_ExpectAndReturn(RCC_USART1EN, PA10, GPIO_Success);

  gpio_pin_config_Expect(PA9, GPIO_Mode_Output_50MHz, GPIO_Output_PushPullAlt);
  gpio_pin_config_Expect(PA10, GPIO_Mode_Input, GPIO_Input_PullUpDown);

  uart_enable_device(UART_Device1, UART1_DEFAULT_MAPPING);
  TEST_ASSERT_TRUE(UART_Instances[UART_Device1].enabled);
  TEST_ASSERT_EQUAL(UART1_DEFAULT_MAPPING, UART_Instances[UART_Device1].chosen_pinconfig);
}

void test_uart_enable_no_such_device_failure(void)
{
  TEST_ASSERT_EQUAL(UART_Err_NoSuchDevice, uart_enable_device(UART_Device2, UART1_DEFAULT_MAPPING));
}

void test_uart_enable_already_enabled_failure(void)
{
  UART_Instances[UART_Device1].enabled = true;
  TEST_ASSERT_EQUAL(UART_Err_AlreadyEnabled, uart_enable_device(UART_Device1, UART1_DEFAULT_MAPPING));
}

void test_uart_enable_invalid_pinconfig_failure(void)
{
  TEST_ASSERT_EQUAL(UART_Err_ConfigFail, uart_enable_device(UART_Device1, 5));
}

void test_uart_enable_config_failure_locked_pin(void)
{
  /* Fail with PA9 already locked */
  rcc_pcendis_Expect(RCC_USART1EN, RCC_ENABLE);
  gpio_port_enable_Expect(GET_GPIO_PORT(PA9));
  gpio_port_enable_Expect(GET_GPIO_PORT(PA10));
  gpio_lock_pin_ExpectAndReturn(RCC_USART1EN, PA9, GPIO_Locked);
  TEST_ASSERT_EQUAL(UART_Err_ConfigFail, uart_enable_device(UART_Device1, UART1_DEFAULT_MAPPING));

  /* Fail with PA10 already locked */
  rcc_pcendis_Expect(RCC_USART1EN, RCC_ENABLE);
  gpio_port_enable_Expect(GET_GPIO_PORT(PA9));
  gpio_port_enable_Expect(GET_GPIO_PORT(PA10));
  gpio_lock_pin_ExpectAndReturn(RCC_USART1EN, PA9, GPIO_Success);
  gpio_lock_pin_ExpectAndReturn(RCC_USART1EN, PA10, GPIO_Locked);
  TEST_ASSERT_EQUAL(UART_Err_ConfigFail, uart_enable_device(UART_Device1, UART1_DEFAULT_MAPPING));
 }

void test_uart_disable_no_such_device_fail(void)
{
  TEST_ASSERT_EQUAL(UART_Err_NoSuchDevice, uart_disable_device(UART_Device2));
}

void test_uart_disable_not_enabled_fail(void)
{
  TEST_ASSERT_EQUAL(UART_Err_NotEnabled, uart_disable_device(UART_Device1));
}

void test_uart_disable_pin_unlock_error_fail(void)
{
  UART_Instances[UART_Device1].enabled = true;
  rcc_pcendis_Expect(RCC_USART1EN, RCC_DISABLE);
  gpio_unlock_pin_ExpectAndReturn(RCC_USART1EN, PA9, GPIO_Ownership);
  TEST_ASSERT_EQUAL(UART_Err_ConfigFail, uart_disable_device(UART_Device1));

  rcc_pcendis_Expect(RCC_USART1EN, RCC_DISABLE);
  gpio_unlock_pin_ExpectAndReturn(RCC_USART1EN, PA9, GPIO_Success);
  gpio_unlock_pin_ExpectAndReturn(RCC_USART1EN, PA10, GPIO_Ownership);
  TEST_ASSERT_EQUAL(UART_Err_ConfigFail, uart_disable_device(UART_Device1));
}

void test_uart_disable_success(void)
{
  UART_Instances[UART_Device1].enabled = true;
  rcc_pcendis_Expect(RCC_USART1EN, RCC_DISABLE);
  gpio_unlock_pin_ExpectAndReturn(RCC_USART1EN, PA9, GPIO_Success);
  gpio_unlock_pin_ExpectAndReturn(RCC_USART1EN, PA10, GPIO_Success);

  TEST_ASSERT_EQUAL(UART_Err_Success, uart_disable_device(UART_Device1));
}

void test_uart_configure_no_such_device_fail(void)
{
  TEST_ASSERT_EQUAL(UART_Err_NoSuchDevice, uart_configure_device(UART_Device2, 9600, 48000000));
}

void test_uart_configure_not_enabled_fail(void)
{
  TEST_ASSERT_EQUAL(UART_Err_NotEnabled, uart_configure_device(UART_Device1, 9600, 48000000));
}

void test_uart_configure_device(void)
{
  // TODO figure out how to ignore aruments to expected function call. This is necessary because
  //      uart_configure_device function is using some static functions to calculate some parameters
  //      e.g. value of BRR register
}

void test_uart_reset_device(void)
{
  // TODO depends on implementing reset infrastructure to RCC subsystem
}

void test_uart_putc(void)
{
  mmio_and_readdw_ExpectAndReturn(_UART_REGISTER(UART_Instances[0].base, R32_UART_STATR), UART_STATR_TC, UART_STATR_TC);
  mmio_writeb_Expect(_UART_REGISTER(UART_Instances[0].base, R32_UART_DATAR), 'U');

  uart_putc(UART_Device1, 'U');
}

void test_uart_getc(void)
{
  mmio_and_readdw_ExpectAndReturn(_UART_REGISTER(UART_Instances[0].base, R32_UART_STATR), UART_STATR_RXNE, UART_STATR_RXNE);
  mmio_readb_ExpectAndReturn(_UART_REGISTER(UART_Instances[0].base, R32_UART_DATAR), 'U');

  TEST_ASSERT_EQUAL('U', uart_getc(UART_Device1));
}
  
int main()
{
  UNITY_BEGIN();
  RUN_TEST(test_uart_enable_success);
  RUN_TEST(test_uart_enable_no_such_device_failure);
  RUN_TEST(test_uart_enable_already_enabled_failure);
  RUN_TEST(test_uart_enable_invalid_pinconfig_failure);
  RUN_TEST(test_uart_enable_config_failure_locked_pin);
  RUN_TEST(test_uart_disable_no_such_device_fail);
  RUN_TEST(test_uart_disable_not_enabled_fail);
  RUN_TEST(test_uart_disable_pin_unlock_error_fail);
  RUN_TEST(test_uart_disable_success);
  RUN_TEST(test_uart_configure_no_such_device_fail);
  RUN_TEST(test_uart_configure_not_enabled_fail);
  RUN_TEST(test_uart_configure_device);
  RUN_TEST(test_uart_reset_device);
  RUN_TEST(test_uart_putc);
  RUN_TEST(test_uart_getc);
  return UNITY_END();
}  
