#include <stdint.h>

#include "spi_interface.h"

#define SPI1_BASE ((uintptr_t)0x40013000)
#define SPI2_BASE ((uintptr_t)0x40003800)
#define SPI3_BASE ((uintptr_t)0x40003C00)

typedef struct
{
  volatile uint16_t CTRL1;
  volatile uint16_t CTRL2;
  volatile uint16_t STATR;
  volatile uint16_t CRCR;
  volatile uint16_t RCRCR;
  volatile uint16_t TCRCR;
  volatile uint16_t I2S_CFGR;
  volatile uint16_t HSCR;
} SPI_Regfile;

/* TODO move following definitions to GPIO file. Or somewhere else  */
#define PORT_DESIGNATOR_MASK (U8_BIT(6) | U8_BIT(7))
#define PORTA_MASK           0
#define PORTB_MASK           U8_BIT(6)
#define PORTC_MASK           U8_BIT(7)
#define PORTD_MASK           (U8_BIT(6) | U8_BIT(7))

#define PIN_PA(x) (x | PORTA_MASK)
#define PIN_PB(x) (x | PORTB_MASK)
#define PIN_PC(x) (x | PORTC_MASK)
#define PIN_PD(x) (x | PORTD_MASK)

#define __SPI(x) ((SPI_Regfile *)x)
