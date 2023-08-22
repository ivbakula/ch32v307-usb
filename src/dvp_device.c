#include "dvp_device.h"

#include "gpio_interface.h"
#include "mmio_ops.h"

extern const bool dvp_instantiated[];
extern const size_t no_of_dvp_instances;
extern DVP_Instance DVP_Instances[];

static uint16_t *buff0_ptr;
static uint16_t *buff1_ptr;

int _printf(const char *fmt, ...);

// TODO implement this function properly. depends on  Implement console #46
void dvp_dump_registers(DVP_Device dev)
{
  /* if (dev >= no_of_dvp_instances) { */
  /*   return; */
  /* } */

  /* if (!dvp_instantiated[dev]) { */
  /*   return; */
  /* } */

  /* uintptr_t base = DVP_Instances[dev].base; */

  /* _printf("DVP_BASE: 0x%08x; base: 0x%08x\r\n", base, DVP_Instances[0].base); */
  /* _printf("===================BEGIN DVP REGISTER DUMP===================\r\n"); */
  /* _printf("R8_DVP_CR0: 0x%02x\r\n", mmio_readb(_DVP_REGISTER(base, R8_DVP_CR0))); */
  /* _printf("R8_DVP_CR1: 0x%02x\r\n", mmio_readb(_DVP_REGISTER(base, R8_DVP_CR1))); */
  /* _printf("R8_DVP_IER: 0x%02x\r\n", mmio_readb(_DVP_REGISTER(base, R8_DVP_IER))); */
  /* _printf("R8_DVP_DMA_BUF0: 0x%08x\r\n", mmio_readb(_DVP_REGISTER(base, R32_DVP_DMA_BUF0))); */
  /* _printf("R8_DVP_DMA_BUF1: 0x%08x\r\n", mmio_readb(_DVP_REGISTER(base, R32_DVP_DMA_BUF1))); */
  /* _printf("R16_DVP_ROW_NUM: 0x%04x\r\n", mmio_readw(_DVP_REGISTER(base, R16_DVP_ROW_NUM))); */
  /* _printf("R16_DVP_COL_NUM: 0x%04x\r\n", mmio_readw(_DVP_REGISTER(base, R16_DVP_COL_NUM))); */
  /* _printf("===================END   DVP REGISTER DUMP===================\r\n"); */
}

#ifdef UNITTEST
void irq_handler_dvp(void)
{
  return;
}
#else
static void irq_handler_dvp(void)
{
  _printf("%s\r\n", __FUNCTION__);
  uint8_t dvp_ifr = mmio_readb(_DVP_REGISTER(DVP_BASE, R8_DVP_IFR));
  uint8_t dvp_clr = 0;
  uint16_t *buff0 = (uint16_t *)mmio_readdw(_DVP_REGISTER(DVP_BASE, R32_DVP_DMA_BUF0));
  uint16_t *buff1 = (uint16_t *)mmio_readdw(_DVP_REGISTER(DVP_BASE, R32_DVP_DMA_BUF1));
  uint16_t rows = mmio_readw(_DVP_REGISTER(DVP_BASE, R16_DVP_ROW_CNT));

  if (dvp_ifr & U8_BIT(0)) {
    _printf("START_FRAME_INTERRUPT\r\n");
    dvp_clr |= U8_BIT(0);
  }

  if (dvp_ifr & U8_BIT(1)) {
    dvp_clr |= U8_BIT(1);
    _printf("ROW_DONE_INTERRUPT\r\n");

    for (int i = 0; i < 10; i++) {
      _printf("buf0[%d]: 0x%04x\r\n", i, buff0_ptr[i]);
    }

    for (int i = 0; i < 10; i++) {
      _printf("buf1[%d]: 0x%04x\r\n", i, buff1_ptr[i]);
    }
  }

  if (dvp_ifr & U8_BIT(2)) {
    _printf("FRAME_DONE_INTERRUPT\r\n");
    dvp_clr |= U8_BIT(2);
  }

  if (dvp_ifr & U8_BIT(3)) {
    _printf("FIFO_OVERFLOW_INTERRUPT\r\n");
    dvp_clr |= U8_BIT(3);
  }

  if (dvp_ifr & U8_BIT(4)) {
    _printf("FRAME_STOP_INTERRUPT\r\n");
    dvp_clr |= U8_BIT(4);
  }

  mmio_and_writeb(_DVP_REGISTER(DVP_BASE, R8_DVP_IFR), ~dvp_clr);
  return;
}
#endif

static void dvp_gpio_unlock_pin_group(RCC_DevId rcc_devid, GPIO_Pin *pins)
{
  for (int i = 0; i < 16; i++)
    gpio_unlock_pin(rcc_devid, pins[i]);
}

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

  rcc_pcendis(rcc_devid, RCC_ENABLE);
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

  buff0_ptr = config.DVP_DMA_BUF0;
  buff1_ptr = config.DVP_DMA_BUF1;

  /* 1. clear bus width setting in control register 0 */
  mmio_and_writeb(_DVP_REGISTER(base, R8_DVP_CR0), ~0x30);

  /* 2. clear FIFO reset and receive logic reset bits in control register 1 */
  mmio_and_writeb(_DVP_REGISTER(base, R8_DVP_CR1), ~(U8_BIT(1) | U8_BIT(2)));

  /* 3. configure DVP receiver device; (bus width, PCLK, HSYNC and VSYNC polarity and data format)
   * Don't enable DVP device for now (the main reason for clearing bit 0)
   */
  mmio_writeb(_DVP_REGISTER(base, R8_DVP_CR0), (config.CR0.data & (~U8_BIT(0))));

  /* 4. configure DVP receiver device; capture mode, crop function, and frame capture rate.
   * Don't enable DMA for now (the main reason for clearing bit 0)
   */
  mmio_or_writeb(_DVP_REGISTER(base, R8_DVP_CR1), (config.CR1.data & (~U8_BIT(0))));

  /* 5. configure number of pixels (samples) per row. */
  mmio_writew(_DVP_REGISTER(base, R16_DVP_COL_NUM), config.DVP_COL_NUM);

  /* 6. configure row numbers per frame */
  mmio_writew(_DVP_REGISTER(base, R16_DVP_ROW_NUM), config.DVP_ROW_NUM);

  /* 7. configure DMA buffer0 */
  mmio_writedw(_DVP_REGISTER(base, R32_DVP_DMA_BUF0), config.DVP_DMA_BUF0);

  /* 8. configure DMA buffer1 */
  mmio_writedw(_DVP_REGISTER(base, R32_DVP_DMA_BUF1), config.DVP_DMA_BUF1);

  /* 9. enable selected interrupts */
  mmio_writeb(_DVP_REGISTER(base, R8_DVP_IER), config.IER.data);

  irq_register_interrupt_handler(irqn, irq_handler_dvp);
  irq_enable_interrupt(irqn);

  DVP_Instances[0].configured = true;

  mmio_or_writeb(_DVP_REGISTER(base, R8_DVP_CR1), config.CR1.DVP_DMA_ENABLE);
  mmio_or_writeb(_DVP_REGISTER(base, R8_DVP_CR0), config.CR0.DVP_ENABLE);

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
