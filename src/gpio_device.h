#ifndef SRC_GPIO_DEVICE_H
#define SRC_GPIO_DEVICE_H

#include "gpio_interface.h"

typedef struct
{
  volatile uint32_t R32_GPIO_CFGLR;
  volatile uint32_t R32_GPIO_CFGHR;
  volatile uint32_t R32_GPIO_INDR;
  volatile uint32_t R32_GPIO_OUTDR;
  volatile uint32_t R32_GPIO_BSHR;
  volatile uint32_t R32_GPIO_BCR;
  volatile uint32_t R32_GPIO_LCKR;
} GPIO_Regfile;

#ifndef UNITTEST

/*************************************/
/* BEGIN PRODUCTION DEFINITIONS      */
/*************************************/
#define GPIOA_BASE ((uintptr_t)0x40010800)
#define GPIOB_BASE ((uintptr_t)0x40010C00)
#define GPIOC_BASE ((uintptr_t)0x40011000)
#define GPIOD_BASE ((uintptr_t)0x40011400)
#define GPIOE_BASE ((uintptr_t)0x40011800)
/*************************************/
/* END PRODUCTION DEFINITIONS        */
/*************************************/

#else

/************************************/
/* BEGIN UNITTEST DEFINITIONS       */
/************************************/
extern GPIO_Regfile gpioa_regfile; /*< Used for mocking of GPIO registers */
extern GPIO_Regfile gpiob_regfile; /*< Used for mocking of GPIO registers */
extern GPIO_Regfile gpioc_regfile; /*< Used for mocking of GPIO registers */
extern GPIO_Regfile gpiod_regfile; /*< Used for mocking of GPIO registers */
extern GPIO_Regfile gpioe_regfile; /*< Used for mocking of GPIO registers */

#define GPIOA_BASE ((uintptr_t)&gpioa_regfile)
#define GPIOB_BASE ((uintptr_t)&gpiob_regfile)
#define GPIOC_BASE ((uintptr_t)&gpioc_regfile)
#define GPIOD_BASE ((uintptr_t)&gpiod_regfile)
#define GPIOE_BASE ((uintptr_t)&gpioe_regfile)

/************************************/
/* END UNITTEST DEFINITIONS         */
/************************************/

#endif

/**
 * @fn _GPIO_REGISTER
 *
 * @brief Calculate register address by using BASE address and offset of a register that is
 *        defined in GPIO_Regfile structure. NOTE: This seems like pretty dirty solution,
 *        it will (probably) be replaceced by something better in the future.
 *
 * @parameter x: MMIO base address of GPIO port
 *
 * @parameter y: Name of the register. Offset of this register is calculated indirectly, by
 *               finding it in UART_Regfile structure.
 */
#define _GPIO_REGISTER(x, y) ((uintptr_t) & (((GPIO_Regfile *)x)->y))

#endif
