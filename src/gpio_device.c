#include "gpio_device.h"
#include "gpio_interface.h"

#ifdef UNITTEST
GPIO_Regfile gpioa_regfile; /*< Used for mocking of GPIO registers */
GPIO_Regfile gpiob_regfile; /*< Used for mocking of GPIO registers */
GPIO_Regfile gpioc_regfile; /*< Used for mocking of GPIO registers */
GPIO_Regfile gpiod_regfile; /*< Used for mocking of GPIO registers */
GPIO_Regfile gpioe_regfile; /*< Used for mocking of GPIO registers */
#endif

void gpio_port_enable(GPIO_Port port)
{
}

void gpio_port_disable(GPIO_Port port)
{
}

GPIO_Err gpio_claim_pin(RCC_DevId rcc_devid, GPIO_Pin pin)
{
  return GPIO_Success;
}

GPIO_Err gpio_free_pin(RCC_DevId rcc_devid, GPIO_Pin pin)
{
  return GPIO_Success;
}

void gpio_pin_config(GPIO_Pin pin, GPIO_Mode mode, GPIO_Config config)
{
}

GPIO_State gpio_pin_input(GPIO_Pin pin)
{
  return GPIO_HIGH;
}

void gpio_pin_output(GPIO_Pin pin, GPIO_State state)
{
}
