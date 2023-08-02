#include <string.h>
#include <unity.h>
#include "gpio_device.h"
#include "rcc_device.h"

extern RCC_Regfile rcc_regfile;

void setUp(void)
{
}

void tearDown(void)
{
  memset(&rcc_regfile, 0, sizeof(rcc_regfile));
}

void test_gpio_port_enable_disable(void)
{
  TEST_ASSERT_EQUAL(rcc_regfile.R32_RCC_APB2PCENR, 0);
  
  gpio_port_enable(GPIO_PORTA);
  TEST_ASSERT_EQUAL(rcc_regfile.R32_RCC_APB2PCENR, U32_BIT(2));

  gpio_port_enable(GPIO_PORTB);
  TEST_ASSERT_EQUAL(rcc_regfile.R32_RCC_APB2PCENR, (U32_BIT(2) | U32_BIT(3)));

  gpio_port_enable(GPIO_PORTC);
  TEST_ASSERT_EQUAL(rcc_regfile.R32_RCC_APB2PCENR, (U32_BIT(2) | U32_BIT(3) | U32_BIT(4)));

  gpio_port_enable(GPIO_PORTD);
  TEST_ASSERT_EQUAL(rcc_regfile.R32_RCC_APB2PCENR, (U32_BIT(2) | U32_BIT(3) | U32_BIT(4) | U32_BIT(5)));

  gpio_port_enable(GPIO_PORTE);
  TEST_ASSERT_EQUAL(rcc_regfile.R32_RCC_APB2PCENR, (U32_BIT(2) | U32_BIT(3) | U32_BIT(4) | U32_BIT(5) | U32_BIT(6)));

  gpio_port_disable(GPIO_PORTE);
  TEST_ASSERT_EQUAL(rcc_regfile.R32_RCC_APB2PCENR, (U32_BIT(2) | U32_BIT(3) | U32_BIT(4) | U32_BIT(5)));

  gpio_port_disable(GPIO_PORTD);
  TEST_ASSERT_EQUAL(rcc_regfile.R32_RCC_APB2PCENR, (U32_BIT(2) | U32_BIT(3) | U32_BIT(4)));

  gpio_port_disable(GPIO_PORTC);
  TEST_ASSERT_EQUAL(rcc_regfile.R32_RCC_APB2PCENR, (U32_BIT(2) | U32_BIT(3)));  

  gpio_port_disable(GPIO_PORTB);
  TEST_ASSERT_EQUAL(rcc_regfile.R32_RCC_APB2PCENR, U32_BIT(2));
  
  gpio_port_disable(GPIO_PORTA);
  TEST_ASSERT_EQUAL(rcc_regfile.R32_RCC_APB2PCENR, 0);
}

void test_gpio_claim_free_pin(void)
{
  TEST_ASSERT_EQUAL(gpio_claim_pin(RCC_SPI1EN, 1), GPIO_Success);
  TEST_ASSERT_EQUAL(gpio_claim_pin(RCC_SPI2EN, 1), GPIO_Claimed);
  TEST_ASSERT_EQUAL(gpio_free_pin(RCC_SPI2EN, 1), GPIO_Ownership);
  TEST_ASSERT_EQUAL(gpio_free_pin(RCC_SPI1EN, 1), GPIO_Success);
}

void test_gpio_pin_config(void)
{
  gpio_pin_config(PA0, GPIO_Mode_Output_50MHz, GPIO_Output_PushPullAlt);
  TEST_ASSERT_EQUAL(gpioa_regfile.R32_GPIO_CFGLR, 0b1011);

  gpio_pin_config(PB5, GPIO_Mode_Input, GPIO_Input_Float);
  TEST_ASSERT_EQUAL(gpiob_regfile.R32_GPIO_CFGLR, (0b0100 << 6));

  gpio_pin_config(PC8, GPIO_Mode_Output_10MHz, GPIO_Output_OpenDrain);
  TEST_ASSERT_EQUAL(gpioc_regfile.R32_GPIO_CFGHR, (0b0101));

  gpio_pin_config(PD13, GPIO_Input_PullUpDown, GPIO_Input_Analog);
  TEST_ASSERT_EQUAL(gpiod_regfile.R32_GPIO_CFGHR, (0b1000 << 6));
}

void test_gpio_pin_output(void)
{
  
}

void test_gpio_pin_input(void)
{
  
}


int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_gpio_port_enable_disable);
  RUN_TEST(test_gpio_claim_free_pin);
  RUN_TEST(test_gpio_pin_config);
  RUN_TEST(test_gpio_pin_output);
  RUN_TEST(test_gpio_pin_input);
  return UNITY_END();
}

