#ifndef SRC_UART_DEVICE_H
#define SRC_UART_DEVICE_H

#include "uart_interface.h"

typedef struct
{
  volatile uint32_t R32_UART_STATR;
  volatile uint32_t R32_UART_DATAR;
  volatile uint32_t R32_UART_BRR;
  volatile uint32_t R32_UART_CTRL1;
  volatile uint32_t R32_UART_CTRL2;
  volatile uint32_t R32_UART_CTRL3;
  volatile uint32_t R32_UART_GPR;
} UART_Regfile;

#ifndef UNITTEST

/*************************************/
/* BEGIN PRODUCTION DEFINITIONS      */
/*************************************/
#define UART1_BASE ((uintptr_t)0x40013800)
#define UART2_BASE ((uintptr_t)0x40004400)
#define UART3_BASE ((uintptr_t)0x40004800)
#define UART4_BASE ((uintptr_t)0x40004C00)
#define UART5_BASE ((uintptr_t)0x40005000)
#define UART6_BASE ((uintptr_t)0x40001800)
#define UART7_BASE ((uintptr_t)0x40001C00)
#define UART8_BASE ((uintptr_t)0x40002000)
/*************************************/
/* END PRODUCTION DEFINITIONS        */
/*************************************/

#else

/************************************/
/* BEGIN UNITTEST DEFINITIONS       */
/************************************/
extern UART_Regfile uart1_regfile;
extern UART_Regfile uart2_regfile;
extern UART_Regfile uart3_regfile;
extern UART_Regfile uart4_regfile;
extern UART_Regfile uart5_regfile;
extern UART_Regfile uart6_regfile;
extern UART_Regfile uart7_regfile;
extern UART_Regfile uart8_regfile;

#define UART1_BASE ((uintptr_t)&uart1_regfile)
#define UART2_BASE ((uintptr_t)&uart2_regfile)
#define UART3_BASE ((uintptr_t)&uart3_regfile)
#define UART4_BASE ((uintptr_t)&uart4_regfile)
#define UART5_BASE ((uintptr_t)&uart5_regfile)
#define UART6_BASE ((uintptr_t)&uart6_regfile)
#define UART7_BASE ((uintptr_t)&uart7_regfile)
#define UART8_BASE ((uintptr_t)&uart8_regfile)
/************************************/
/* END UNITTEST DEFINITIONS         */
/************************************/

#endif

/**
 * @fn _UART_REGISTER
 *
 * @brief Calculate register address by using BASE address and offset of a register that is
 *        defined in UART_Regfile structure. NOTE: This seems like pretty dirty solution,
 *        it will (probably) be replaceced by something better in the future.
 *
 * @parameter X: MMIO base address of UART device
 *
 * @parameter Y: Name of the register. Offset of this register is calculated indirectly, by
 *               finding it in UART_Regfile structure.
 */
#define _UART_REGISTER(X, Y) ((uintptr_t) & (((UART_Regfile *)X)->Y))

#endif
