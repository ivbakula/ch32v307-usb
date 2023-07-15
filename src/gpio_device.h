#ifndef SRC_GPIO_DEVICE_H
#define SRC_GPIO_DEVICE_H
#include <stdint.h>

/* GPIO stuff */
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

#define GPIOA_BASE ((uintptr_t)0x40010800)
#define GPIOB_BASE ((uintptr_t)0x40010C00)
#define GPIOC_BASE ((uintptr_t)0x40011000)
#define GPIOD_BASE ((uintptr_t)0x40011400)
#define GPIOE_BASE ((uintptr_t)0x40011800)

#define GPIO_CFGLR_OFFSET ((uintptr_t)0x00)
#define GPIO_CFGHR_OFFSET ((uintptr_t)0x04)
#define GPIO_INDR_OFFSET  ((uintptr_t)0x08)
#define GPIO_OUTDR_OFFSET ((uintptr_t)0x0c)
#define GPIO_BSHR_OFFSET  ((uintptr_t)0x10)
#define GPIO_BCR_OFFSET   ((uintptr_t)0x14)
#define GPIO_LCKR_OFFSET  ((uintptr_t)0x18)

/* output mode bits */
#define GPIO_GEN_PP_OUT 0b00
#define GPIO_GEN_OD_OUT 0b01
#define GPIO_ALT_PP_OUT 0b10
#define GPIO_ALT_OD_OUT 0b11

/* input mode bits */
#define GPIO_ANALOG_IN 0b00
#define GPIO_FLOAT_IN  0b01
#define GPIO_PP_IN     0b10
#define GPIO_RESERVED  0b11
#endif
