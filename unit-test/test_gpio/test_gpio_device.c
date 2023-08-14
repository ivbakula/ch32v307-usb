#include <unity.h>

#include "Mockmmio_ops.h"
#include "Mockrcc_interface.h"
#include "gpio_device.h"
#include "helpers.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_gpio_port_enable_disable(void)
{
  rcc_pcendis_Expect(RCC_IOPAEN, RCC_ENABLE);
  gpio_port_enable(GPIO_PORTA);

  rcc_pcendis_Expect(RCC_IOPAEN, RCC_DISABLE);
  gpio_port_disable(GPIO_PORTA);

  rcc_pcendis_Expect(RCC_IOPBEN, RCC_ENABLE);
  gpio_port_enable(GPIO_PORTB);

  rcc_pcendis_Expect(RCC_IOPBEN, RCC_DISABLE);
  gpio_port_disable(GPIO_PORTB);

  rcc_pcendis_Expect(RCC_IOPCEN, RCC_ENABLE);
  gpio_port_enable(GPIO_PORTC);

  rcc_pcendis_Expect(RCC_IOPCEN, RCC_DISABLE);
  gpio_port_disable(GPIO_PORTC);

  rcc_pcendis_Expect(RCC_IOPDEN, RCC_ENABLE);
  gpio_port_enable(GPIO_PORTD);

  rcc_pcendis_Expect(RCC_IOPDEN, RCC_DISABLE);
  gpio_port_disable(GPIO_PORTD);

  rcc_pcendis_Expect(RCC_IOPEEN, RCC_ENABLE);
  gpio_port_enable(GPIO_PORTE);

  rcc_pcendis_Expect(RCC_IOPEEN, RCC_DISABLE);
  gpio_port_disable(GPIO_PORTE);
}

void test_gpio_lock_unlock_pin(void)
{
  TEST_ASSERT_EQUAL(gpio_lock_pin(RCC_SPI1EN, 1), GPIO_Success);
  TEST_ASSERT_EQUAL(gpio_lock_pin(RCC_SPI2EN, 1), GPIO_Locked);
  TEST_ASSERT_EQUAL(gpio_unlock_pin(RCC_SPI2EN, 1), GPIO_Ownership);
  TEST_ASSERT_EQUAL(gpio_unlock_pin(RCC_SPI1EN, 1), GPIO_Success);
}

void test_gpio_pin_config(void)
{
  const uint32_t output_mode_50mhz_push_pull_alt = 0b1011;
  const uint32_t input_mode_float = 0b0100;
  const uint32_t output_mode_10mhz_open_drain = 0b0101;
  const uint32_t input_mode_pull_up_down = 0b1000;

  /* CFGLR */
  const uint32_t pa0_shift = 0;
  const uint32_t pa0_mask = 0x000f;
  const uint32_t pb5_shift = 20;
  const uint32_t pb5_mask = (0xf << pb5_shift);
  /* CFGHR */
  const uint32_t pc8_shift = 0;
  const uint32_t pc8_mask = 0x000f;
  const uint32_t pd13_shift = 20;
  const uint32_t pd13_mask = (0x000f << pd13_shift);

  mmio_and_writedw_Expect(_GPIO_REGISTER(GPIOA_BASE, R32_GPIO_CFGLR), ~pa0_mask);
  mmio_or_writedw_Expect(_GPIO_REGISTER(GPIOA_BASE, R32_GPIO_CFGLR), output_mode_50mhz_push_pull_alt << pa0_shift);
  gpio_pin_config(PA0, GPIO_Mode_Output_50MHz, GPIO_Output_PushPullAlt);

  mmio_and_writedw_Expect(_GPIO_REGISTER(GPIOB_BASE, R32_GPIO_CFGLR), ~pb5_mask);
  mmio_or_writedw_Expect(_GPIO_REGISTER(GPIOB_BASE, R32_GPIO_CFGLR), input_mode_float << pb5_shift);
  gpio_pin_config(PB5, GPIO_Mode_Input, GPIO_Input_Float);

  mmio_and_writedw_Expect(_GPIO_REGISTER(GPIOC_BASE, R32_GPIO_CFGHR), ~pc8_mask);
  mmio_or_writedw_Expect(_GPIO_REGISTER(GPIOC_BASE, R32_GPIO_CFGHR), output_mode_10mhz_open_drain << pc8_shift);
  gpio_pin_config(PC8, GPIO_Mode_Output_10MHz, GPIO_Output_OpenDrain);

  mmio_and_writedw_Expect(_GPIO_REGISTER(GPIOD_BASE, R32_GPIO_CFGHR), ~pd13_mask);
  mmio_or_writedw_Expect(_GPIO_REGISTER(GPIOD_BASE, R32_GPIO_CFGHR), input_mode_pull_up_down << pd13_shift);
  gpio_pin_config(PD13, GPIO_Mode_Input, GPIO_Input_PullUpDown);
}

void test_gpio_pin_output(void)
{
  mmio_or_writew_Expect(_GPIO_REGISTER(GPIOA_BASE, R32_GPIO_OUTDR), U16_BIT(15));
  gpio_pin_output(PA15, GPIO_HIGH);

  mmio_and_writew_Expect(_GPIO_REGISTER(GPIOA_BASE, R32_GPIO_OUTDR), ~U16_BIT(15));
  gpio_pin_output(PA15, GPIO_LOW);
}

void test_gpio_pin_input(void)
{
  mmio_and_readw_ExpectAndReturn(_GPIO_REGISTER(GPIOA_BASE, R32_GPIO_INDR), U16_BIT(15), U16_BIT(15));
  TEST_ASSERT_EQUAL(1, gpio_pin_input(PA15));
}

int main()
{
  UNITY_BEGIN();
  RUN_TEST(test_gpio_port_enable_disable);
  RUN_TEST(test_gpio_lock_unlock_pin);
  RUN_TEST(test_gpio_pin_config);
  RUN_TEST(test_gpio_pin_output);
  RUN_TEST(test_gpio_pin_input);
  return UNITY_END();
}
