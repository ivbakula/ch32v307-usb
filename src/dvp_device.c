#include "dvp_device.h"

#include "gpio_interface.h"
#include "mmio_ops.h"

#ifdef UNITTEST
void irq_handler_dvp(void)
{
  return;
}
#else
static void irq_handler_dvp(void)
{
  return;
}
#endif

static void dvp_gpio_unlock_pin_group(RCC_DevId rcc_devid, GPIO_Pin *pins)
{
  for (int i = 0; i < 16; i++)
    gpio_unlock_pin(rcc_devid, pins[i]);
}

extern const bool dvp_instantiated[];
extern const size_t no_of_dvp_instances;
extern DVP_Instance DVP_Instances[];

DVP_Err dvp_enable_device(DVP_Device dev, DVP_PinConfig pin_config)
{
  if (dev >= no_of_dvp_instances)
    return DVP_Err_DeviceOutOfRange;

  if (!dvp_instantiated[dev])
    return DVP_Err_NoSuchDevice;

  DVP_Instance *inst = &DVP_Instances[0];

  if (inst->enabled)
    return DVP_Err_AlreadyEnabled;

  if (pin_config >= inst->no_pin_configs)
    return DVP_Err_ConfigFail;

  GPIO_Pin *pins = inst->pin_configuration[pin_config];
  RCC_DevId rcc_devid = inst->rcc_devid;
  inst->chosen_pinconfig = pin_config;

  for (int i = 0; i < 16; i++) {
    gpio_port_enable(GET_GPIO_PORT(pins[i]));
    if (gpio_lock_pin(rcc_devid, pins[i])) {
      dvp_gpio_unlock_pin_group(rcc_devid, pins);
      return DVP_Err_ConfigFail;
    }
    gpio_pin_config(pins[i], GPIO_Mode_Input, GPIO_Input_PullUpDown);
    gpio_pin_pullstate_config(pins[i], GPIO_PullState_Up);
  }

  inst->enabled = true;
  return DVP_Err_Success;
}

DVP_Err dvp_disable_device(DVP_Device dev)
{
  if (dev >= no_of_dvp_instances)
    return DVP_Err_DeviceOutOfRange;

  if (!dvp_instantiated[dev])
    return DVP_Err_NoSuchDevice;

  DVP_Instance *inst = &DVP_Instances[0];

  if (!inst->enabled)
    return DVP_Err_NotEnabled;

  uint8_t pin_config = inst->chosen_pinconfig;
  GPIO_Pin *pins = inst->pin_configuration[pin_config];
  RCC_DevId rcc_devid = inst->rcc_devid;
  uintptr_t base = inst->base;

  mmio_and_readb(_DVP_REGISTER(base, R8_DVP_CR1), ~U8_BIT(0)); /* Disable DMA for DVP device */
  mmio_and_readb(_DVP_REGISTER(base, R8_DVP_CR0), ~U8_BIT(0)); /* Disable DVP device */
  rcc_pcendis(rcc_devid, RCC_DISABLE);
  dvp_gpio_unlock_pin_group(rcc_devid, pins);
}

DVP_Err dvp_reset_device(DVP_Device dev)
{
  // TODO
}

DVP_Err dvp_configure_device(DVP_Device dev, DVP_Config config)
{
  if (dev >= no_of_dvp_instances)
    return DVP_Err_DeviceOutOfRange;

  if (!dvp_instantiated[dev])
    return DVP_Err_NoSuchDevice;

  DVP_Instance *inst = &DVP_Instances[0];

  if (!inst->enabled)
    return DVP_Err_NotEnabled;

  if (inst->configured)
    return DVP_Err_AlreadyConfigured;

  uintptr_t base = inst->base;
  IRQn_Type irqn = inst->irqn;

  mmio_and_writeb(_DVP_REGISTER(base, R8_DVP_CR1), ~(U8_BIT(1) | U8_BIT(2)));
  mmio_writeb(_DVP_REGISTER(base, R8_DVP_CR0), config.CR0.data & (~U8_BIT(0)));
  mmio_writeb(_DVP_REGISTER(base, R8_DVP_CR1), config.CR1.data & (~U8_BIT(0)));
  mmio_writeb(_DVP_REGISTER(base, R8_DVP_IER), config.IER.data);
  mmio_writew(_DVP_REGISTER(base, R16_DVP_COL_NUM), config.DVP_COL_NUM);
  mmio_writew(_DVP_REGISTER(base, R16_DVP_ROW_NUM), config.DVP_ROW_NUM);
  mmio_writedw(_DVP_REGISTER(base, R32_DVP_DMA_BUF0), config.DVP_DMA_BUF0);
  mmio_writedw(_DVP_REGISTER(base, R32_DVP_DMA_BUF1), config.DVP_DMA_BUF1);

  irq_register_interrupt_handler(irqn, irq_handler_dvp);
  irq_enable_interrupt(irqn);

  DVP_Instances[0].configured = true;

  mmio_or_writeb(_DVP_REGISTER(base, R8_DVP_CR1), U8_BIT(0)); /* Enable DMA channel */
  mmio_or_writeb(_DVP_REGISTER(base, R8_DVP_CR0), U8_BIT(0)); /* Enable DVP interface */

  return DVP_Err_Success;
}

DVP_Err dvp_is_enabled(DVP_Device dev)
{
  if (dev >= no_of_dvp_instances)
    return DVP_Err_DeviceOutOfRange;

  if (!dvp_instantiated[dev])
    return DVP_Err_NoSuchDevice;

  return DVP_Instances[dev].enabled;
}

DVP_Err dvp_is_configured(DVP_Device dev)
{
  if (dev >= no_of_dvp_instances)
    return DVP_Err_DeviceOutOfRange;

  if (!dvp_instantiated[dev])
    return DVP_Err_NoSuchDevice;

  return DVP_Instances[dev].configured;
}
