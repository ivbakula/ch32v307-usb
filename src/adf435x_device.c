#include "adf435x_device.h"

#include <stddef.h>
#include <stdbool.h>
#include "adf435x_interface.h"
#include "gpio_interface.h"
#include "time.h"

static SPI_Device spi_iface;
static bool is_binded;
static bool is_enabled;
static size_t spi_transfer_size;
static GPIO_Pin LE;
static GPIO_Pin CE;
static GPIO_Pin LD;

extern size_t adf435x_noof_instances;
extern bool adf435x_instantiated[];
extern ADF435x_Instance ADF435x_Instances[];

ADF435x_Err adf435x_create_interface(SPI_Device spi_bus, ADF435x_PinConfig pin_config)
{
  if (!spi_is_enabled(spi_bus))
    return ADF435x_Err_ConfigFail;

  if (!spi_is_configured(spi_bus))
    return ADF435x_Err_ConfigFail;

  spi_iface = spi_bus;

  // TODO lock GPIOs. This one depends on Issue: Implement better pin locking #30
  gpio_port_enable(GET_GPIO_PORT(pin_config.LE));
  gpio_pin_config(pin_config.LE, GPIO_Mode_Output_50MHz, GPIO_Output_PushPull);
  LE = pin_config.LE;

  gpio_port_enable(GET_GPIO_PORT(pin_config.CE));
  gpio_pin_config(pin_config.CE, GPIO_Mode_Output_50MHz, GPIO_Output_PushPull);
  CE = pin_config.CE;

  gpio_port_enable(GET_GPIO_PORT(pin_config.LD));
  gpio_pin_config(pin_config.LD, GPIO_Mode_Output_50MHz, GPIO_Input_PullUpDown);
  LD = pin_config.LD;

  gpio_pin_output(CE, GPIO_HIGH);
  gpio_pin_output(LE, GPIO_HIGH);

  spi_transfer_size = spi_packet_size(spi_iface);
  is_created = true;
  return ADF435x_Err_Success;
}

static void unlock_pin_group(uint32_t uuid, const GPIO_Pin *pins)
{
  for (int i = 0; i < 3; i++)
    gpio_unlock_pin(uuid, pins[i]);
}

ADF435x_Err adf435x_bind_device(ADF435x_Device dev, SPI_Device spi_bus, ADF435x_PinConfig pin_config)
{
  if (dev > adf435x_noof_instances - 1)
    return ADF435x_Err_NoSuchDevice;

  if (adf435x_instantiated[dev])
    return ADF435x_Err_NoSuchDevice;
  
  if (!spi_is_enabled(spi_bus))
    return ADF435x_Err_ConfigFail;

  if (!spi_is_configured(spi_bus))
    return ADF435x_Err_ConfigFail;

  ADF435x_Instance *inst = &ADF435x_Instances[dev];

  if (pin_config > inst->no_pin_config - 1)
    return ADF435x_Err_ConfigFail;

  const GPIO_Pin *chosen_pin_config = inst->pin_configuration[pin_config];

  // TODO After Issue #30 Implement better pin locing is implemented, apply it here.
  // for now, just use RCC ID of GPIO portlock GPIOs.
  for (int i = 0; i < 3; i++) {
    gpio_port_enable(GET_GPIO_PORT(chosen_pin_config[i]));

    // TODO use bulk lock after Issue #40
    if (gpio_lock_pin(inst->uuid, chosen_pin_config[i])) {
      unlock_pin_group(inst->uuid, chosen_pin_config);
      return ADF435x_Err_ConfigFail;
    }
  }

  for (int i = 0; i < 3; i++) {
    if (i == ADF435x_LD_INDEX) {
      gpio_pin_config(chosen_pin_config[i], GPIO_Mode_Input, GPIO_Input_PullUpDown);
    } else {
      gpio_pin_config(chosen_pin_config[i], GPIO_Mode_Output_50MHz, GPIO_Output_PushPull);
    }
  }

  inst->spi_bus = spi_bus;  
  inst->spi_transfer_size = spi_packet_size(spi_iface);
  inst->is_binded = true;
  
  return ADF435x_Err_Success;
}

ADF435x_Err adf435x_enable_device(ADF435x_Device dev)
{
  if (dev > adf435x_noof_instances - 1)
    return ADF435x_Err_NoSuchDevice;

  ADF435x_Instance *inst = &ADF435x_Instances[dev];

  if (!inst->is_binded)
    return ADF435x_Err_NotBinded;

  ADF435x_PinConfig pin_config = inst->chosen_pinconfig;
  const GPIO_Pin *chosen_pin_config = inst->pin_configuration[pin_config];

  gpio_pin_output(chosen_pin_config[ADF435x_CE_INDEX], GPIO_HIGH);
  gpio_pin_output(chosen_pin_config[ADF435x_LE_INDEX], GPIO_HIGH);
  inst->is_enabled = true;

  return ADF435x_Err_Success;  
}

ADF435x_Err adf435x_disable_device(ADF435x_Device dev)
{
  if (dev > adf435x_noof_instances - 1)
    return ADF435x_Err_NoSuchDevice;

  ADF435x_Instance *inst = &ADF435x_Instances[dev];

  if (!inst->is_binded)
    return ADF435x_Err_NotBinded;
  
  if (!inst->is_enabled)
    return ADF435x_Err_NotEnabled;

  ADF435x_PinConfig pin_config = inst->chosen_pinconfig;
  const GPIO_Pin *chosen_pin_config = inst->pin_configuration[pin_config];

  gpio_pin_output(chosen_pin_config[ADF435x_CE_INDEX], GPIO_LOW);
  gpio_pin_output(chosen_pin_config[ADF435x_LE_INDEX], GPIO_LOW);
  inst->is_enabled = false;

  return ADF435x_Err_Success;
}

static void adf435x_write_register(uint32_t reg)
{
  ADF435x_RegisterHelper helper = {.reg = reg};

  gpio_pin_output(LE, GPIO_LOW);
  for (int i = 0; i < 4; i++) {
    spi_write(spi_iface, helper.r8[3 - i]);
  }

  wait_us(5);
  gpio_pin_output(LE, GPIO_HIGH);
}

ADF435x_Err adf435x_program_device(ADF435x_Regs regs)
{
  if (!is_created)
    return ADF435x_Err_NoSuchDevice;

  for (int i = 0; i < 6; i++) {
    adf435x_write_register(regs.adf435x_regs[6 - 1 - i]);
  }

  while (gpio_pin_input(LD))
    ;
  return ADF435x_Err_Success;
}
