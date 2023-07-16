#include <stdint.h>
#include "rcc_devids.h"
#include "rcc_interface.h"
#include "spi_device.h"
#include "mmem.h"
#include "spi_interface.h"

static const uint32_t spi_rccdevid_mapping[] = {
  RCC_SPI1EN,
  RCC_SPI2EN,
  RCC_SPI3EN,
};

static const uintptr_t spi_mmio_mapping[] = {
  SPI1_BASE,
  SPI2_BASE,
  SPI3_BASE,
};

struct SPI_GPIOPort {
  int len;
  uint32_t gpio_port_rccid[4];
};

static const struct SPI_GPIOPort spi_gpio_port_mapping[] = {
  {1, {RCC_IOPAEN, 0}},
  {0, {0, 0}},
  {2, {RCC_IOPAEN, RCC_IOPBEN, 0}},
  {},
};

struct GPIO_Pins {
  uint8_t nss;
  uint8_t sck;
  uint8_t miso;
  uint8_t mosi;
};

static const struct GPIO_Pins spi_gpio_pin_mapping[] = {
  /* SPI1 GPIO pinout */
  {.nss = PIN_PA(4), .sck = PIN_PA(5), .miso = PIN_PA(6), .mosi = PIN_PA(7)},
  /* SPI2 GPIO pinout (unknown, have to check in datasheet ) */
  {0, 0, 0, 0},
  /* SPI3 GPIO pinout */
  {.nss = PIN_PA(15), .sck = PIN_PB(3), .miso = PIN_PB(4), .mosi = PIN_PB(5)},
};

DeviceDescriptor *spi_create_dd(SPI_Device dev_id)
{
  if (dev_id >= SPI_Max)
    return 0;
  
  DeviceDescriptor *dev = (DeviceDescriptor *)allocm(sizeof(*dev));

  if (!dev)
    return dev;

  dev->mmio_base = spi_mmio_mapping[dev_id];
  dev->device_id = DEV_SPI;
  dev->rcc_id = spi_rccdevid_mapping[dev_id];
  dev->instance = dev_id;
  dev->ready = false;
}

void spi_enable_dev(DeviceDescriptor *dev)
{
  if (!dev)
    return;


  rcc_enable_periph(dev->rcc_id);

  /* Enable GPIO port */
  const struct SPI_GPIOPort *ports = &spi_gpio_port_mapping[dev->instance];

  for (int i = 0; i < ports->len; i++) {
    rcc_enable_periph(ports->gpio_port_rccid);
  }
}  
