#include "uart_device.h"

#include "gpio_interface.h"
#include "irq.h"
#include "mmio_ops.h"
#include "rcc_interface.h"

// instantiate uart drivers (NEEDS to be the last header included)

#ifdef UNITTEST
UART_Regfile uart1_regfile;
UART_Regfile uart2_regfile;
UART_Regfile uart3_regfile;
UART_Regfile uart4_regfile;
UART_Regfile uart5_regfile;
UART_Regfile uart6_regfile;
UART_Regfile uart7_regfile;
UART_Regfile uart8_regfile;
#endif

extern unsigned char uart_instantiated[8];
extern UART_Instance UART_Instances[8];

// TODO: in _enable function, only select pin configuration. Actual pin options
// shall be set in configure routine. Here, you should only enable RCC to both
// GPIO port and to actual UART device
UART_Err uart_enable_device(UART_Device dev, UART_PinConfig pin_config)
{
  if (!uart_instantiated[dev])
    return UART_Err_NoSuchDevice;

  UART_Instance *inst = &UART_Instances[dev];

  if (inst->enabled)
    return UART_Err_AlreadyEnabled;

  if (pin_config > inst->no_pin_configs - 1)
    return UART_Err_ConfigFail;

  inst->chosen_pinconfig = pin_config;
  RCC_DevId rcc_devid = inst->rcc_devid;
  GPIO_Pin tx = inst->pin_configuration[pin_config][UART_TX_PIN_INDEX];
  GPIO_Pin rx = inst->pin_configuration[pin_config][UART_RX_PIN_INDEX];

  rcc_pcendis(rcc_devid, RCC_ENABLE);
  gpio_port_enable(GET_GPIO_PORT(tx));
  gpio_port_enable(GET_GPIO_PORT(rx));

  if (gpio_lock_pin(rcc_devid, tx))
    return UART_Err_ConfigFail;

  if (gpio_lock_pin(rcc_devid, rx))
    return UART_Err_ConfigFail;

  gpio_pin_config(tx, GPIO_Mode_Output_50MHz, GPIO_Output_PushPullAlt);
  gpio_pin_config(rx, GPIO_Mode_Input, GPIO_Input_PullUpDown);

  inst->enabled = true;
  return UART_Err_Success;
}

UART_Err uart_disable_device(UART_Device dev)
{
  if (!uart_instantiated[dev])
    return UART_Err_NoSuchDevice;

  UART_Instance *inst = &UART_Instances[dev];

  if (!inst->enabled)
    return UART_Err_NotEnabled;

  RCC_DevId rcc_devid = inst->rcc_devid;
  uint8_t chosen_pinconfig = inst->chosen_pinconfig;
  GPIO_Pin tx = inst->pin_configuration[chosen_pinconfig][UART_TX_PIN_INDEX];
  GPIO_Pin rx = inst->pin_configuration[chosen_pinconfig][UART_RX_PIN_INDEX];
  rcc_pcendis(rcc_devid, RCC_DISABLE);

  if (gpio_unlock_pin(rcc_devid, tx))
    return UART_Err_ConfigFail;

  if (gpio_unlock_pin(rcc_devid, rx))
    return UART_Err_ConfigFail;

  inst->enabled = false;
  return UART_Err_Success;
}

UART_Err uart_reset_device(UART_Device dev)
{
  // TODO: Not in the scope for now. But implement After issue #18 Add reset support in RCC subsystem
  return UART_Err_Success;
}

static uint16_t calculate_brr(uint32_t apb_clock, uint32_t uart_baud_rate)
{
  uint16_t over8div = 4;
  uint16_t integer_divider = (25 * (apb_clock)) / ((over8div) * (uart_baud_rate));
  uint16_t fractional_divider = integer_divider % 100;

  return ((((integer_divider) / 100) << 4) | (((((fractional_divider) * ((over8div)*2)) + 50) / 100) & 7));
}

static void irq_uartx_handler(void)
{
}

UART_Err uart_configure_device(UART_Device dev, uint32_t baud, uint32_t apbclk)
{
  uintptr_t base;
  UART_Instance *inst;

  if (!uart_instantiated[dev])
    return UART_Err_NoSuchDevice;

  inst = &UART_Instances[dev];
  base = inst->base;

  if (!inst->enabled)
    return UART_Err_NotEnabled;

  mmio_writedw(_UART_REGISTER(base, R32_UART_CTRL1), UART_CTRL1_TX_ENABLE | UART_CTRL1_RX_ENABLE);
  mmio_writedw(_UART_REGISTER(base, R32_UART_CTRL2), 0);
  mmio_writedw(_UART_REGISTER(base, R32_UART_CTRL3), 0);
  mmio_writew(_UART_REGISTER(base, R32_UART_BRR), calculate_brr(apbclk, baud));
  mmio_or_writedw(_UART_REGISTER(base, R32_UART_CTRL1), /*UART_CTRL1_RXNEIE |*/ UART_CTRL1_ENABLE);

#ifndef UNITTEST
  irq_register_interrupt_handler(USART1_IRQn, irq_uartx_handler);
  irq_enable_interrupt(USART1_IRQn);
#endif
  return UART_Err_Success;
}

void uart_putc(UART_Device dev, char c)
{
  if (!uart_instantiated[dev])
    return;

  uintptr_t base = UART_Instances[dev].base;
  while (!mmio_and_readdw(_UART_REGISTER(base, R32_UART_STATR), UART_STATR_TC))
    ;

  mmio_writeb(_UART_REGISTER(base, R32_UART_DATAR), c);
}

char uart_getc(UART_Device dev)
{
  if (!uart_instantiated[dev])
    return -1;

  uintptr_t base = UART_Instances[dev].base;

  while (!mmio_and_readdw(_UART_REGISTER(base, R32_UART_STATR), UART_STATR_RXNE))
    ;

  return mmio_readb(_UART_REGISTER(base, R32_UART_DATAR));
}
