#ifndef INC_SPI_INTERFACE_H
#define INC_SPI_INTERFACE_H

#include "device_descriptor.h"

typedef enum
{
  SPI1,
  SPI2,
  SPI3,
  SPI_Max,
} SPI_Device;

DeviceDescriptor *spi_create_dd(SPI_Device dev);

void spi_enable_dev(DeviceDescriptor *spi_dev);

void spi_init_dev(DeviceDescriptor *spi_dev);

void spi_write_u8(DeviceDescriptor *spi_dev, uint8_t data);
void spi_write_u16(DeviceDescriptor *spi_dev, uint16_t data);
uint8_t spi_read_u8(DeviceDescriptor *spi_dev);
uint16_t spi_read_u16(DeviceDescriptor *spi_dev);


#endif
