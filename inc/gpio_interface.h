#ifndef INC_GPIO_INTERFACE
#define INC_GPIO_INTERFACE

#include <stdint.h>

#include "rcc_interface.h"

typedef uint32_t GPIO_Pin;
typedef uint32_t GPIO_Port;

/**
 * @name GPIO Port definitions
 * @brief
 */
///@{
#define GPIO_PORT_MASK         ((GPIO_Pin)0xe0000000)
#define GPIO_PORTA             ((GPIO_Port)(0 << 29))
#define GPIO_PORTB             ((GPIO_Port)(1 << 29))
#define GPIO_PORTC             ((GPIO_Port)(2 << 29))
#define GPIO_PORTD             ((GPIO_Port)(3 << 29))
#define GPIO_PORTE             ((GPIO_Port)(4 << 29))
#define GET_GPIO_PORT(x)       ((GPIO_Port)(GPIO_PORT_MASK & x))
#define GET_GPIO_PORT_INDEX(x) ((GPIO_PORT_MASK & x) >> 29)
///@}

/**
 * @name GPIO pin ordinal number
 */
///@{
#define ORD_PX_MASK ((uint32_t)(0xf << 25))
#define ORD_PX0     ((uint32_t)(0x0 << 25))
#define ORD_PX1     ((uint32_t)(0x1 << 25))
#define ORD_PX2     ((uint32_t)(0x2 << 25))
#define ORD_PX3     ((uint32_t)(0x3 << 25))
#define ORD_PX4     ((uint32_t)(0x4 << 25))
#define ORD_PX5     ((uint32_t)(0x5 << 25))
#define ORD_PX6     ((uint32_t)(0x6 << 25))
#define ORD_PX7     ((uint32_t)(0x7 << 25))
#define ORD_PX8     ((uint32_t)(0x8 << 25))
#define ORD_PX9     ((uint32_t)(0x9 << 25))
#define ORD_PX10    ((uint32_t)(0xa << 25))
#define ORD_PX11    ((uint32_t)(0xb << 25))
#define ORD_PX12    ((uint32_t)(0xc << 25))
#define ORD_PX13    ((uint32_t)(0xd << 25))
#define ORD_PX14    ((uint32_t)(0xe << 25))
#define ORD_PX15    ((uint32_t)(0xf << 25))

#define GET_GPIO_PIN_INDEX(x) ((x & ORD_PX_MASK) >> 25)
///@}

/**
 * @name PIN_DEFINITIONS
 * @brief
 */
///@{
#define PA0  ((GPIO_Pin)(GPIO_PORTA | ORD_PX0 | U32_BIT(0)))
#define PA1  ((GPIO_Pin)(GPIO_PORTA | ORD_PX1 | U32_BIT(1)))
#define PA2  ((GPIO_Pin)(GPIO_PORTA | ORD_PX2 | U32_BIT(2)))
#define PA3  ((GPIO_Pin)(GPIO_PORTA | ORD_PX3 | U32_BIT(3)))
#define PA4  ((GPIO_Pin)(GPIO_PORTA | ORD_PX4 | U32_BIT(4)))
#define PA5  ((GPIO_Pin)(GPIO_PORTA | ORD_PX5 | U32_BIT(5)))
#define PA6  ((GPIO_Pin)(GPIO_PORTA | ORD_PX6 | U32_BIT(6)))
#define PA7  ((GPIO_Pin)(GPIO_PORTA | ORD_PX7 | U32_BIT(7)))
#define PA8  ((GPIO_Pin)(GPIO_PORTA | ORD_PX8 | U32_BIT(8)))
#define PA9  ((GPIO_Pin)(GPIO_PORTA | ORD_PX9 | U32_BIT(9)))
#define PA10 ((GPIO_Pin)(GPIO_PORTA | ORD_PX10 | U32_BIT(10)))
#define PA11 ((GPIO_Pin)(GPIO_PORTA | ORD_PX11 | U32_BIT(11)))
#define PA12 ((GPIO_Pin)(GPIO_PORTA | ORD_PX12 | U32_BIT(12)))
#define PA13 ((GPIO_Pin)(GPIO_PORTA | ORD_PX13 | U32_BIT(13)))
#define PA14 ((GPIO_Pin)(GPIO_PORTA | ORD_PX14 | U32_BIT(14)))
#define PA15 ((GPIO_Pin)(GPIO_PORTA | ORD_PX15 | U32_BIT(15)))

#define PB0  ((GPIO_Pin)(GPIO_PORTB | ORD_PX0 | U32_BIT(0)))
#define PB1  ((GPIO_Pin)(GPIO_PORTB | ORD_PX1 | U32_BIT(1)))
#define PB2  ((GPIO_Pin)(GPIO_PORTB | ORD_PX2 | U32_BIT(2)))
#define PB3  ((GPIO_Pin)(GPIO_PORTB | ORD_PX3 | U32_BIT(3)))
#define PB4  ((GPIO_Pin)(GPIO_PORTB | ORD_PX4 | U32_BIT(4)))
#define PB5  ((GPIO_Pin)(GPIO_PORTB | ORD_PX5 | U32_BIT(5)))
#define PB6  ((GPIO_Pin)(GPIO_PORTB | ORD_PX6 | U32_BIT(6)))
#define PB7  ((GPIO_Pin)(GPIO_PORTB | ORD_PX7 | U32_BIT(7)))
#define PB8  ((GPIO_Pin)(GPIO_PORTB | ORD_PX8 | U32_BIT(8)))
#define PB9  ((GPIO_Pin)(GPIO_PORTB | ORD_PX9 | U32_BIT(9)))
#define PB10 ((GPIO_Pin)(GPIO_PORTB | ORD_PX10 | U32_BIT(10)))
#define PB11 ((GPIO_Pin)(GPIO_PORTB | ORD_PX11 | U32_BIT(11)))
#define PB12 ((GPIO_Pin)(GPIO_PORTB | ORD_PX12 | U32_BIT(12)))
#define PB13 ((GPIO_Pin)(GPIO_PORTB | ORD_PX13 | U32_BIT(13)))
#define PB14 ((GPIO_Pin)(GPIO_PORTB | ORD_PX14 | U32_BIT(14)))
#define PB15 ((GPIO_Pin)(GPIO_PORTB | ORD_PX15 | U32_BIT(15)))

#define PC0  ((GPIO_Pin)(GPIO_PORTC | ORD_PX0 | U32_BIT(0)))
#define PC1  ((GPIO_Pin)(GPIO_PORTC | ORD_PX1 | U32_BIT(1)))
#define PC2  ((GPIO_Pin)(GPIO_PORTC | ORD_PX2 | U32_BIT(2)))
#define PC3  ((GPIO_Pin)(GPIO_PORTC | ORD_PX3 | U32_BIT(3)))
#define PC4  ((GPIO_Pin)(GPIO_PORTC | ORD_PX4 | U32_BIT(4)))
#define PC5  ((GPIO_Pin)(GPIO_PORTC | ORD_PX5 | U32_BIT(5)))
#define PC6  ((GPIO_Pin)(GPIO_PORTC | ORD_PX6 | U32_BIT(6)))
#define PC7  ((GPIO_Pin)(GPIO_PORTC | ORD_PX7 | U32_BIT(7)))
#define PC8  ((GPIO_Pin)(GPIO_PORTC | ORD_PX8 | U32_BIT(8)))
#define PC9  ((GPIO_Pin)(GPIO_PORTC | ORD_PX9 | U32_BIT(9)))
#define PC10 ((GPIO_Pin)(GPIO_PORTC | ORD_PX10 | U32_BIT(10)))
#define PC11 ((GPIO_Pin)(GPIO_PORTC | ORD_PX11 | U32_BIT(11)))
#define PC12 ((GPIO_Pin)(GPIO_PORTC | ORD_PX12 | U32_BIT(12)))
#define PC13 ((GPIO_Pin)(GPIO_PORTC | ORD_PX13 | U32_BIT(13)))
#define PC14 ((GPIO_Pin)(GPIO_PORTC | ORD_PX14 | U32_BIT(14)))
#define PC15 ((GPIO_Pin)(GPIO_PORTC | ORD_PX15 | U32_BIT(15)))

#define PD0  ((GPIO_Pin)(GPIO_PORTD | ORD_PX0 | U32_BIT(0)))
#define PD1  ((GPIO_Pin)(GPIO_PORTD | ORD_PX1 | U32_BIT(1)))
#define PD2  ((GPIO_Pin)(GPIO_PORTD | ORD_PX2 | U32_BIT(2)))
#define PD3  ((GPIO_Pin)(GPIO_PORTD | ORD_PX3 | U32_BIT(3)))
#define PD4  ((GPIO_Pin)(GPIO_PORTD | ORD_PX4 | U32_BIT(4)))
#define PD5  ((GPIO_Pin)(GPIO_PORTD | ORD_PX5 | U32_BIT(5)))
#define PD6  ((GPIO_Pin)(GPIO_PORTD | ORD_PX6 | U32_BIT(6)))
#define PD7  ((GPIO_Pin)(GPIO_PORTD | ORD_PX7 | U32_BIT(7)))
#define PD8  ((GPIO_Pin)(GPIO_PORTD | ORD_PX8 | U32_BIT(8)))
#define PD9  ((GPIO_Pin)(GPIO_PORTD | ORD_PX9 | U32_BIT(9)))
#define PD10 ((GPIO_Pin)(GPIO_PORTD | ORD_PX10 | U32_BIT(10)))
#define PD11 ((GPIO_Pin)(GPIO_PORTD | ORD_PX11 | U32_BIT(11)))
#define PD12 ((GPIO_Pin)(GPIO_PORTD | ORD_PX12 | U32_BIT(12)))
#define PD13 ((GPIO_Pin)(GPIO_PORTD | ORD_PX13 | U32_BIT(13)))
#define PD14 ((GPIO_Pin)(GPIO_PORTD | ORD_PX14 | U32_BIT(14)))
#define PD15 ((GPIO_Pin)(GPIO_PORTD | ORD_PX15 | U32_BIT(15)))

#define PE0  ((GPIO_Pin)(GPIO_PORTE | ORD_PX0 | U32_BIT(0)))
#define PE1  ((GPIO_Pin)(GPIO_PORTE | ORD_PX1 | U32_BIT(1)))
#define PE2  ((GPIO_Pin)(GPIO_PORTE | ORD_PX2 | U32_BIT(2)))
#define PE3  ((GPIO_Pin)(GPIO_PORTE | ORD_PX3 | U32_BIT(3)))
#define PE4  ((GPIO_Pin)(GPIO_PORTE | ORD_PX4 | U32_BIT(4)))
#define PE5  ((GPIO_Pin)(GPIO_PORTE | ORD_PX5 | U32_BIT(5)))
#define PE6  ((GPIO_Pin)(GPIO_PORTE | ORD_PX6 | U32_BIT(6)))
#define PE7  ((GPIO_Pin)(GPIO_PORTE | ORD_PX7 | U32_BIT(7)))
#define PE8  ((GPIO_Pin)(GPIO_PORTE | ORD_PX8 | U32_BIT(8)))
#define PE9  ((GPIO_Pin)(GPIO_PORTE | ORD_PX9 | U32_BIT(9)))
#define PE10 ((GPIO_Pin)(GPIO_PORTE | ORD_PX10 | U32_BIT(10)))
#define PE11 ((GPIO_Pin)(GPIO_PORTE | ORD_PX11 | U32_BIT(11)))
#define PE12 ((GPIO_Pin)(GPIO_PORTE | ORD_PX12 | U32_BIT(12)))
#define PE13 ((GPIO_Pin)(GPIO_PORTE | ORD_PX13 | U32_BIT(13)))
#define PE14 ((GPIO_Pin)(GPIO_PORTE | ORD_PX14 | U32_BIT(14)))
#define PE15 ((GPIO_Pin)(GPIO_PORTE | ORD_PX15 | U32_BIT(15)))

#define PNA ((GPIO_Pin)(0xffffffff)) /* N/A pin */

#define GET_GPIO_PIN_BIT(x) ((uint16_t)(x & 0xffff))
///@}

typedef enum
{
  GPIO_Mode_Input,
  GPIO_Mode_Output_10MHz,
  GPIO_Mode_Output_2MHz,
  GPIO_Mode_Output_50MHz
} GPIO_Mode;

typedef enum
{
  GPIO_Input_Analog = 0,
  GPIO_Input_Float = 1,
  GPIO_Input_PullUpDown = 2,

  GPIO_Output_PushPull = 0,
  GPIO_Output_OpenDrain = 1,
  GPIO_Output_PushPullAlt = 2,
  GPIO_Output_OpenDrainAlt = 3
} GPIO_Config;

/* BEGIN: typedefs and enums just for lolz */
typedef uint8_t GPIO_State;
#define GPIO_HIGH ((GPIO_State)1)
#define GPIO_LOW  ((GPIO_State)0)
/* END: typedefs an enums just for lolz */

typedef enum
{
  GPIO_Success,   /*< GPIO pin successfuly claimed */
  GPIO_Locked,    /*< Requested GPIO pin is already taken. */
  GPIO_Ownership, /*< GPIO pin is not claimed by provided RCC_DevId, so pin cannot be freed */
} GPIO_Err;

typedef enum
  {
    GPIO_PullState_Up,
    GPIO_PullState_Down
  } GPIO_PullState; 
/**
 * @fn gpio_port_enable
 *
 * @brief Peripherial clock enable for GPIO port.
 *
 * @param port: GPIO port that we wish to enable.
 */
void gpio_port_enable(GPIO_Port port);

/**
 * @fn gpio_port_disable
 *
 * @brief Peripherial clock disable for GPIO port.
 *
 * @param port: GPIO port that we wish to disable.
 */
void gpio_port_disable(GPIO_Port port);

/**
 * @fn gpio_lock_pin
 *
 * @brief Lock specific GPIO pin from a bank; This operation is usually performed by device driver. After
 *        successful invocation of this function, requested pin belongs to the device with provided
 *        rcc_devid, and it shouldn't be for anything or anyone except the entity that locked it.
 *        If locked pin is not needed any more, it shall be unlocked. Unlock operation is preformed with
 *        \gpio_unlock_pin\ function.
 *
 * @param rcc_devid: RCC Device ID of peripherial device that is trying to lock the pin. For devices
 *                   not residing within MCU, GPIO bank RCC_DevId shall be provided.
 *
 * @param pin: Pin from GPIO bank. Check section PIN_DEFINITIONS for available options
 *
 * @return GPIO_Err, GPIO_Success if operation was successfuly completed, error code otherwise.
 */
GPIO_Err gpio_lock_pin(RCC_DevId rcc_devid, GPIO_Pin pin);

/**
 * @fn gpio_unlock_pin
 *
 * @brief Unlock previously locked GPIO pin.
 *
 * @param rcc_devid: RCC Device ID of peripherial device that is trying to unlock the pin. For devices
 *                   not residing within MCU, GPIO bank RCC_DevId shall be provided.
 *
 * @param GPIO_Err, GPIO_Success if operation was successfuly completed, error code otherwise.
 */
GPIO_Err gpio_unlock_pin(RCC_DevId rcc_devid, GPIO_Pin pin);

/**
 * @fn gpio_pin_config
 *
 * @brief Configure GPIO pin.
 *
 * @param pin: Desired pin
 *
 * @param mode:
 *
 * @param config:
 */
void gpio_pin_config(GPIO_Pin pin, GPIO_Mode mode, GPIO_Config config);

void gpio_pin_pullstate_config(GPIO_Pin pin, GPIO_PullState state);
/**
 * @fn gpio_pin_input
 *
 * @brief Receive input from GPIO pin.
 *
 * @param pin: GPIO pin to read input from.
 *
 * @return GPIO_HIGH or GPIO_LOW
 */
GPIO_State gpio_pin_input(GPIO_Pin pin);

/**
 * @fn gpio_pin_output
 *
 * @brief Output bit to GPIO pin.
 *
 * @param pin: GPIO pin to write output to.
 *
 * @param state GPIO_HIGH, GPIO_LOW
 */
void gpio_pin_output(GPIO_Pin pin, GPIO_State state);

#endif
