#include "spi_device.h"

#include "gpio_device.h"
#include "gpio_interface.h"
#include "mmio_ops.h"
#include "rcc_interface.h"
#include "spi_interface.h"

#ifdef UNITTEST
SPI_Regfile spi1_regfile;
SPI_Regfile spi2_regfile;
SPI_Regfile spi3_regfile;
#endif

extern SPI_Instance SPI_Instances[3];
extern bool spi_instantiated[3];

/* Claim and configure GPIO pins for Master SDDM TX mode SPI */
static SPI_Err spi_gpio_pin_configuration_master_sddm_tx_mode(RCC_DevId rcc_devid, GPIO_Pin *pins)
{
  for (int i = 0; i < SPI_MISO_INDEX; i++) {
    gpio_pin_config(i, GPIO_Mode_Output_50MHz, GPIO_Output_PushPullAlt);
  }

  return SPI_Err_Success;
}

static void spi_gpio_unlock_pin_group(RCC_DevId rcc_devid, GPIO_Pin *pins)
{
  for (int i = 0; i < 4; i++)
    gpio_unlock_pin(rcc_devid, pins[i]);
}

// TODO: in _enable function, only select pin configuration. Actual pin options
// shall be set in configure routine. Here, you should only enable RCC to both
// GPIO port and to actual SPI device
SPI_Err spi_enable_device(SPI_Device dev, SPI_PinConfig pin_config)
{
  if (!spi_instantiated[dev])
    return SPI_Err_NoSuchDevice;

  SPI_Instance *inst = &SPI_Instances[dev];

  if (inst->enabled)
    return SPI_Err_AlreadyEnabled;

  if (pin_config > inst->no_pin_configs)
    return SPI_Err_ConfigFail;

  inst->chosen_pinconfig = pin_config;
  RCC_DevId rcc_devid = inst->rcc_devid;

  rcc_pcendis(rcc_devid, RCC_ENABLE);

  /* Lock SPI pins */
  for (int i = 0; i < 4; i++) {
    gpio_port_enable(GET_GPIO_PORT(inst->pin_configuration[pin_config][i]));

    // if requested pin is already locked by another device, unlock all previously locked pins and
    // return CONFIGURATION ERROR.
    if (gpio_lock_pin(rcc_devid, inst->pin_configuration[pin_config][i])) {
      spi_gpio_unlock_pin_group(rcc_devid, &inst->pin_configuration[pin_config][0]);
      return SPI_Err_ConfigFail;
    }
  }

  inst->enabled = true;
  return SPI_Err_Success;
}

SPI_Err spi_disable_device(SPI_Device dev)
{
  if (!spi_instantiated[dev])
    return SPI_Err_NoSuchDevice;

  SPI_Instance *inst = &SPI_Instances[dev];

  if (!inst->enabled)
    return SPI_Err_NotEnabled;

  RCC_DevId rcc_devid = inst->rcc_devid;
  uint8_t pin_config = inst->chosen_pinconfig;

  rcc_pcendis(rcc_devid, RCC_DISABLE);

  for (int i = 0; i < 4; i++) {
    gpio_unlock_pin(rcc_devid, inst->pin_configuration[pin_config][i]);
  }

  inst->enabled = false;
  return SPI_Err_Success;
}

// TODO
SPI_Err spi_reset_device(SPI_Device dev)
{
  return SPI_Err_Success;
}

SPI_Err spi_configure_device(SPI_Device dev, SPI_Config config)
{
  if (!spi_instantiated[dev])
    return SPI_Err_NoSuchDevice;

  SPI_Instance *inst = &SPI_Instances[dev];
  if (!inst->enabled)
    return SPI_Err_NotEnabled;

  uintptr_t base = inst->base;
  RCC_DevId rcc_devid = inst->rcc_devid;
  uint8_t pin_config = inst->chosen_pinconfig;

  // This is Master mode SDDM TX configuration.
  // TODO add handlers for other configurations
  if (config.CTRL1._MSTR == 1 && config.CTRL1._SDDMEN == 1 && config.CTRL1._SDDM_TX == 1) {
    for (int i = 0; i < 4; i++) {
      gpio_pin_config(inst->pin_configuration[pin_config][i], GPIO_Mode_Output_50MHz, GPIO_Output_PushPullAlt);
    }

    // we don't need MISO pin, so unlock it
    gpio_unlock_pin(rcc_devid, inst->pin_configuration[pin_config][SPI_MISO_INDEX]);
  }

  mmio_writew(_SPI_REGISTER(base, CTRL1), config.CTRL1.data);
  mmio_writew(_SPI_REGISTER(base, CTRL2), config.CTRL2.data);

  inst->packet_sz = (config.CTRL1._DFF) ? SPI_PACKET_TRANSFER_SIZE_8 : SPI_PACKET_TRANSFER_SIZE_16;

  inst->configured = true;
  return SPI_Err_Success;
}

void spi_write(SPI_Device dev, uint16_t data)
{
  uintptr_t base = SPI_Instances[dev].base;
  while (!(mmio_and_readw(_SPI_REGISTER(base, STATR), U16_BIT(1))))
    ;

  mmio_writew(_SPI_REGISTER(base, DATAR), data);
}

uint16_t spi_read(SPI_Device dev)
{
  if (!spi_instantiated[dev])
    return -1;

  SPI_Instance *inst = &SPI_Instances[dev];
  if (!inst->enabled)
    return -1;

  if (!inst->configured)
    return -1;

  uintptr_t base = inst->base;
  uint8_t packet_sz = inst->packet_sz;
  while (!(mmio_and_readw(_SPI_REGISTER(base, STATR), U16_BIT(0))))
    ;

  if (packet_sz == 8)
    return mmio_and_readb(_SPI_REGISTER(base, DATAR), 0xff);

  return mmio_readw(_SPI_REGISTER(base, DATAR));
}

bool spi_is_enabled(SPI_Device dev)
{
  if (!spi_instantiated[dev])
    return SPI_Err_NoSuchDevice;

  return SPI_Instances[dev].enabled;
}

bool spi_is_configured(SPI_Device dev)
{
  if (!spi_instantiated[dev])
    return SPI_Err_NoSuchDevice;

  return SPI_Instances[dev].configured;
}

uint8_t spi_packet_size(SPI_Device dev)
{
  if (!spi_instantiated[dev])
    return SPI_Err_NoSuchDevice;

  return SPI_Instances[dev].packet_sz;
}

void spi_wait_tx(SPI_Device dev)
{
  if (!spi_instantiated[dev])
    return;

  uintptr_t base = SPI_Instances[dev].base;

  while (!mmio_and_readw(_SPI_REGISTER(base, STATR), U16_BIT(1)))
    ;
}
