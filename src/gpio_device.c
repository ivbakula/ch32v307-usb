#include "gpio_device.h"
#include "gpio_interface.h"
#include "mmio.h"

static const uintptr_t __gpio_iface_mmio[] = {GPIOA_BASE, GPIOB_BASE, GPIOC_BASE, GPIOD_BASE, GPIOE_BASE};
static const uintptr_t __gpio_cfg_mapping[] = {
  /* Output */
  GPIO_GEN_PP_OUT,
  GPIO_GEN_OD_OUT,
  GPIO_ALT_PP_OUT,
  GPIO_ALT_OD_OUT,
  /* Input */
  GPIO_ANALOG_IN,
  GPIO_FLOAT_IN,
  GPIO_PP_IN,
  GPIO_RESERVED,
};


void gpio_pin_config(GPIO_Iface gpio_port, uint8_t pin, GPIO_Config config)
{
  uint8_t shift = 0;
  uint32_t *reg = 0;

  if (pin > 15)
    return;

  if (pin < 8) {
    shift = pin << 2;
    reg = (uint32_t *)(__gpio_iface_mmio[gpio_port] + GPIO_CFGLR_OFFSET);
  } else {
    shift = (pin - 8) << 2;
    reg = (uint32_t *)(__gpio_iface_mmio[gpio_port] + GPIO_CFGHR_OFFSET);
  }

  
  mmio_write_wmask_u32(reg, shift | U32_BIT(shift+1), __gpio_cfg_mapping[config]);
}

void gpio_pin_write(GPIO_Iface gpio_port, uint8_t pin, GPIO_Config config)
{
  
}  
