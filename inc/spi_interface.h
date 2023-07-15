#ifndef SPI_INTERFACE
#define SPI_INTERFACE

#include <stdint.h>

void spi_init(void);
void spi_write(uint32_t data);

#endif
