#include "gpio_interface.h"
#include "irq.h"
#include "rcc_interface.h"
#include "spi_interface.h"

typedef struct
{
  volatile uint16_t CTRL1;
  volatile uint16_t padding_1;
  volatile uint16_t CTRL2;
  volatile uint16_t padding_2;
  volatile uint16_t STATR;
  volatile uint16_t padding_3;
  volatile uint16_t DATAR;
  volatile uint16_t padding_4;
  volatile uint16_t CRCR;
  volatile uint16_t padding_5;
  volatile uint16_t RCRCR;
  volatile uint16_t padding_6;
  volatile uint16_t TCRCR;
  volatile uint16_t padding_7;
  volatile uint16_t I2S_CFGR;
  volatile uint16_t padding_8;
  volatile uint16_t HSCR;
} SPI_Regfile;

typedef struct _SPI_Instance
{
  uintptr_t base;
  RCC_DevId rcc_devid;
  Interrupt_IRQn irqn;
  bool enabled;
  bool configured;
  uint8_t chosen_pinconfig;
  uint8_t no_pin_configs;
  GPIO_Pin pin_configuration[2][4];
} SPI_Instance;

#define SPI_NSS_INDEX  0
#define SPI_SCK_INDEX  1
#define SPI_MOSI_INDEX 2
#define SPI_MISO_INDEX 3

#ifndef UNITTEST

/*************************************/
/* BEGIN PRODUCTION DEFINITIONS      */
/*************************************/
#define SPI1_BASE ((uintptr_t)0x40013000)
#define SPI2_BASE ((uintptr_t)0x40003800)
#define SPI3_BASE ((uintptr_t)0x40003C00)
/*************************************/
/* END PRODUCTION DEFINITIONS        */
/*************************************/

#else

/************************************/
/* BEGIN UNITTEST DEFINITIONS       */
/************************************/
extern SPI_Regfile spi1_regfile;
extern SPI_Regfile spi2_regfile;
extern SPI_Regfile spi3_regfile;

#define SPI1_BASE ((uintptr_t)&spi1_regfile)
#define SPI2_BASE ((uintptr_t)&spi2_regfile)
#define SPI3_BASE ((uintptr_t)&spi3_regfile)
/************************************/
/* END UNITTEST DEFINITIONS         */
/************************************/

#endif
