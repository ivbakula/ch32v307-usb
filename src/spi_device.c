#include "spi_device.h"

#ifdef UNITTEST
SPI_Regfile spi1_regfile;
SPI_Regfile spi2_regfile;
SPI_Regfile spi3_regfile;
#endif

SPI_Err spi_enable_device(SPI_Device dev, SPI_PinConfig pin_config)
{
  return SPI_Err_Success;
}

SPI_Err spi_disable_device(SPI_Device dev)
{
  return SPI_Err_Success;
}

SPI_Err spi_reset_device(SPI_Device dev)
{
  return SPI_Err_Success;
}

SPI_Err spi_configure_device(SPI_Device dev, SPI_Config config)
{
  return SPI_Err_Success;
}

void spi_write_u8(SPI_Device dev, uint8_t data)
{
}

void spi_write_u16(SPI_Device dev, uint16_t data)
{
}

uint8_t spi_read_u8(SPI_Device dev)
{
  return 0;
}

uint16_t spi_read_u16(SPI_Device dev)
{
  return 0;
}
