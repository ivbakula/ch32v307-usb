#include "gpio_device.h"

#include "gpio_interface.h"
#include "mmio_ops.h"
#include "rcc_interface.h"

#define ERROR ((uint32_t)~0)

#ifdef UNITTEST
GPIO_Regfile gpioa_regfile; /*< Used for mocking of GPIO registers */
GPIO_Regfile gpiob_regfile; /*< Used for mocking of GPIO registers */
GPIO_Regfile gpioc_regfile; /*< Used for mocking of GPIO registers */
GPIO_Regfile gpiod_regfile; /*< Used for mocking of GPIO registers */
GPIO_Regfile gpioe_regfile; /*< Used for mocking of GPIO registers */
#endif

static uint32_t __attribute__((section(".data"))) gpio_pin_lock[5][16] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* GPIOA PIN locks */
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* GPIOB PIN locks */
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* GPIOC PIN locks */
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* GPIOD PIN locks */
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* GPIOE PIN locks */
};

static RCC_DevId gpio_get_port_rcc_mapping(GPIO_Port port)
{
  switch (GET_GPIO_PORT(port)) {
    case GPIO_PORTA:
      return RCC_IOPAEN;
    case GPIO_PORTB:
      return RCC_IOPBEN;
    case GPIO_PORTC:
      return RCC_IOPCEN;
    case GPIO_PORTD:
      return RCC_IOPDEN;
    case GPIO_PORTE:
      return RCC_IOPEEN;
    default:
      // not supported. Do something about this case
      return -1;
  }
}

void gpio_port_enable(GPIO_Port port)
{
  RCC_DevId devid = gpio_get_port_rcc_mapping(port);
  if (devid == ERROR)
    // OOPS something went wrong. You provided invalid GPIO port.
    // TODO: do something about this
    return;

  rcc_pcendis(devid, RCC_ENABLE);
}

void gpio_port_disable(GPIO_Port port)
{
  RCC_DevId devid = gpio_get_port_rcc_mapping(port);
  if (devid == ERROR)
    // OOPS something went wrong. You provided invalid GPIO port.
    // TODO: do something about this
    return;

  rcc_pcendis(devid, RCC_DISABLE);
}

GPIO_Err gpio_lock_pin(RCC_DevId rcc_devid, GPIO_Pin pin)
{
  uint32_t port_index = GET_GPIO_PORT_INDEX(pin);
  uint32_t pin_index = GET_GPIO_PIN_INDEX(pin);

  RCC_DevId is_locked = gpio_pin_lock[port_index][pin_index];
  if (is_locked)
    return GPIO_Locked;

  gpio_pin_lock[port_index][pin_index] = rcc_devid;
  return GPIO_Success;
}

GPIO_Err gpio_unlock_pin(RCC_DevId rcc_devid, GPIO_Pin pin)
{
  uint32_t port_index = GET_GPIO_PORT_INDEX(pin);
  uint32_t pin_index = GET_GPIO_PIN_INDEX(pin);
  RCC_DevId owner = gpio_pin_lock[port_index][pin_index];

  if (owner != rcc_devid)
    return GPIO_Ownership;

  gpio_pin_lock[port_index][pin_index] = 0;
  return GPIO_Success;
}

static uintptr_t gpio_get_port_base_address_mapping(GPIO_Pin pin)
{
  switch (GET_GPIO_PORT(pin)) {
    case GPIO_PORTA:
      return GPIOA_BASE;
    case GPIO_PORTB:
      return GPIOB_BASE;
    case GPIO_PORTC:
      return GPIOC_BASE;
    case GPIO_PORTD:
      return GPIOD_BASE;
    case GPIO_PORTE:
      return GPIOE_BASE;
    default:
      // OOPS this should never happen. How???
      // TODO do something about it.
      return ERROR;
  }
}

void gpio_pin_config(GPIO_Pin pin, GPIO_Mode mode, GPIO_Config config)
{
  uint8_t shift = 0;
  uintptr_t reg = 0;
  uint8_t val = mode | (config << 2);
  uint8_t pin_index = GET_GPIO_PIN_INDEX(pin);
  uintptr_t base = gpio_get_port_base_address_mapping(pin);

  if (pin_index < 8) {
    shift = pin_index << 2;
    reg = (uintptr_t)(_GPIO_REGISTER(base, R32_GPIO_CFGLR));
  } else {
    shift = (pin_index - 8) << 2;
    reg = (uintptr_t)(_GPIO_REGISTER(base, R32_GPIO_CFGHR));
  }

  mmio_and_writedw(reg, ~(0xf << shift));
  mmio_or_writedw(reg, ((0xf & val) << shift));
  //  *reg &= ~(0xf << shift);
  //  *reg |= (0xf & val) << shift;
}
void gpio_pin_pullstate_config(GPIO_Pin pin, GPIO_PullState state)
{
  uint8_t pin_index = GET_GPIO_PIN_INDEX(pin);
  uintptr_t base = gpio_get_port_base_address_mapping(pin);

  if (state == GPIO_PullState_Up) {
    mmio_writew(_GPIO_REGISTER(base, R32_GPIO_BSHR), GET_GPIO_PIN_BIT(pin));
  } else {
    mmio_writew(_GPIO_REGISTER(base, R32_GPIO_BCR), GET_GPIO_PIN_BIT(pin));
  }
}

GPIO_State gpio_pin_input(GPIO_Pin pin)
{
  uintptr_t port_base = gpio_get_port_base_address_mapping(pin);
  uint8_t pin_index = GET_GPIO_PIN_INDEX(pin);

  return (mmio_and_readw(_GPIO_REGISTER(port_base, R32_GPIO_INDR), U16_BIT(pin_index)) >> pin_index);
}

void gpio_pin_output(GPIO_Pin pin, GPIO_State state)
{
  uintptr_t port_base = gpio_get_port_base_address_mapping(pin);
  uint8_t pin_index = GET_GPIO_PIN_INDEX(pin);

  if (state)
    mmio_or_writew(_GPIO_REGISTER(port_base, R32_GPIO_OUTDR), U16_BIT(pin_index));
  else
    mmio_and_writew(_GPIO_REGISTER(port_base, R32_GPIO_OUTDR), ~U16_BIT(pin_index));
}
