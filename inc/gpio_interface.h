#ifndef INC_GPIO_INTERFACE
#define INC_GPIO_INTERFACE

#include <stdint.h>

#include "rcc_interface.h"

typedef uint32_t GPIO_Pin;
typedef uint32_t GPIO_Port;

#define GPIO_PORT_MASK ((GPIO_Pin)0xe0000000)
#define GPIO_PORTA     ((GPIO_Port)(1 << 29))
#define GPIO_PORTB     ((GPIO_Port)(2 << 29))
#define GPIO_PORTC     ((GPIO_Port)(3 << 29))
#define GPIO_PORTD     ((GPIO_Port)(4 << 29))
#define GPIO_PORTE     ((GPIO_Port)(5 << 29))

/**
 * @name PIN_DEFINITIONS
 * @brief
 */
///@{
#define PA0  ((GPIO_Pin)(GPIO_PORTA | U32_BIT(0)))
#define PA1  ((GPIO_Pin)(GPIO_PORTA | U32_BIT(1)))
#define PA2  ((GPIO_Pin)(GPIO_PORTA | U32_BIT(2)))
#define PA3  ((GPIO_Pin)(GPIO_PORTA | U32_BIT(3)))
#define PA4  ((GPIO_Pin)(GPIO_PORTA | U32_BIT(4)))
#define PA5  ((GPIO_Pin)(GPIO_PORTA | U32_BIT(5)))
#define PA6  ((GPIO_Pin)(GPIO_PORTA | U32_BIT(6)))
#define PA7  ((GPIO_Pin)(GPIO_PORTA | U32_BIT(7)))
#define PA8  ((GPIO_Pin)(GPIO_PORTA | U32_BIT(8)))
#define PA9  ((GPIO_Pin)(GPIO_PORTA | U32_BIT(9)))
#define PA10 ((GPIO_Pin)(GPIO_PORTA | U32_BIT(10)))
#define PA11 ((GPIO_Pin)(GPIO_PORTA | U32_BIT(11)))
#define PA12 ((GPIO_Pin)(GPIO_PORTA | U32_BIT(12)))
#define PA13 ((GPIO_Pin)(GPIO_PORTA | U32_BIT(13)))
#define PA14 ((GPIO_Pin)(GPIO_PORTA | U32_BIT(14)))
#define PA15 ((GPIO_Pin)(GPIO_PORTA | U32_BIT(15)))

#define PB0  ((GPIO_Pin)(GPIO_PORTB | U32_BIT(0)))
#define PB1  ((GPIO_Pin)(GPIO_PORTB | U32_BIT(1)))
#define PB2  ((GPIO_Pin)(GPIO_PORTB | U32_BIT(2)))
#define PB3  ((GPIO_Pin)(GPIO_PORTB | U32_BIT(3)))
#define PB4  ((GPIO_Pin)(GPIO_PORTB | U32_BIT(4)))
#define PB5  ((GPIO_Pin)(GPIO_PORTB | U32_BIT(5)))
#define PB6  ((GPIO_Pin)(GPIO_PORTB | U32_BIT(6)))
#define PB7  ((GPIO_Pin)(GPIO_PORTB | U32_BIT(7)))
#define PB8  ((GPIO_Pin)(GPIO_PORTB | U32_BIT(8)))
#define PB9  ((GPIO_Pin)(GPIO_PORTB | U32_BIT(9)))
#define PB10 ((GPIO_Pin)(GPIO_PORTB | U32_BIT(10)))
#define PB11 ((GPIO_Pin)(GPIO_PORTB | U32_BIT(11)))
#define PB12 ((GPIO_Pin)(GPIO_PORTB | U32_BIT(12)))
#define PB13 ((GPIO_Pin)(GPIO_PORTB | U32_BIT(13)))
#define PB14 ((GPIO_Pin)(GPIO_PORTB | U32_BIT(14)))
#define PB15 ((GPIO_Pin)(GPIO_PORTB | U32_BIT(15)))

#define PC0  ((GPIO_Pin)(GPIO_PORTC | U32_BIT(0)))
#define PC1  ((GPIO_Pin)(GPIO_PORTC | U32_BIT(1)))
#define PC2  ((GPIO_Pin)(GPIO_PORTC | U32_BIT(2)))
#define PC3  ((GPIO_Pin)(GPIO_PORTC | U32_BIT(3)))
#define PC4  ((GPIO_Pin)(GPIO_PORTC | U32_BIT(4)))
#define PC5  ((GPIO_Pin)(GPIO_PORTC | U32_BIT(5)))
#define PC6  ((GPIO_Pin)(GPIO_PORTC | U32_BIT(6)))
#define PC7  ((GPIO_Pin)(GPIO_PORTC | U32_BIT(7)))
#define PC8  ((GPIO_Pin)(GPIO_PORTC | U32_BIT(8)))
#define PC9  ((GPIO_Pin)(GPIO_PORTC | U32_BIT(9)))
#define PC10 ((GPIO_Pin)(GPIO_PORTC | U32_BIT(10)))
#define PC11 ((GPIO_Pin)(GPIO_PORTC | U32_BIT(11)))
#define PC12 ((GPIO_Pin)(GPIO_PORTC | U32_BIT(12)))
#define PC13 ((GPIO_Pin)(GPIO_PORTC | U32_BIT(13)))
#define PC14 ((GPIO_Pin)(GPIO_PORTC | U32_BIT(14)))
#define PC15 ((GPIO_Pin)(GPIO_PORTC | U32_BIT(15)))

#define PD0  ((GPIO_Pin)(GPIO_PORTD | U32_BIT(0)))
#define PD1  ((GPIO_Pin)(GPIO_PORTD | U32_BIT(1)))
#define PD2  ((GPIO_Pin)(GPIO_PORTD | U32_BIT(2)))
#define PD3  ((GPIO_Pin)(GPIO_PORTD | U32_BIT(3)))
#define PD4  ((GPIO_Pin)(GPIO_PORTD | U32_BIT(4)))
#define PD5  ((GPIO_Pin)(GPIO_PORTD | U32_BIT(5)))
#define PD6  ((GPIO_Pin)(GPIO_PORTD | U32_BIT(6)))
#define PD7  ((GPIO_Pin)(GPIO_PORTD | U32_BIT(7)))
#define PD8  ((GPIO_Pin)(GPIO_PORTD | U32_BIT(8)))
#define PD9  ((GPIO_Pin)(GPIO_PORTD | U32_BIT(9)))
#define PD10 ((GPIO_Pin)(GPIO_PORTD | U32_BIT(10)))
#define PD11 ((GPIO_Pin)(GPIO_PORTD | U32_BIT(11)))
#define PD12 ((GPIO_Pin)(GPIO_PORTD | U32_BIT(12)))
#define PD13 ((GPIO_Pin)(GPIO_PORTD | U32_BIT(13)))
#define PD14 ((GPIO_Pin)(GPIO_PORTD | U32_BIT(14)))
#define PD15 ((GPIO_Pin)(GPIO_PORTD | U32_BIT(15)))

#define PE0  ((GPIO_Pin)(GPIO_PORTE | U32_BIT(0)))
#define PE1  ((GPIO_Pin)(GPIO_PORTE | U32_BIT(1)))
#define PE2  ((GPIO_Pin)(GPIO_PORTE | U32_BIT(2)))
#define PE3  ((GPIO_Pin)(GPIO_PORTE | U32_BIT(3)))
#define PE4  ((GPIO_Pin)(GPIO_PORTE | U32_BIT(4)))
#define PE5  ((GPIO_Pin)(GPIO_PORTE | U32_BIT(5)))
#define PE6  ((GPIO_Pin)(GPIO_PORTE | U32_BIT(6)))
#define PE7  ((GPIO_Pin)(GPIO_PORTE | U32_BIT(7)))
#define PE8  ((GPIO_Pin)(GPIO_PORTE | U32_BIT(8)))
#define PE9  ((GPIO_Pin)(GPIO_PORTE | U32_BIT(9)))
#define PE10 ((GPIO_Pin)(GPIO_PORTE | U32_BIT(10)))
#define PE11 ((GPIO_Pin)(GPIO_PORTE | U32_BIT(11)))
#define PE12 ((GPIO_Pin)(GPIO_PORTE | U32_BIT(12)))
#define PE13 ((GPIO_Pin)(GPIO_PORTE | U32_BIT(13)))
#define PE14 ((GPIO_Pin)(GPIO_PORTE | U32_BIT(14)))
#define PE15 ((GPIO_Pin)(GPIO_PORTE | U32_BIT(15)))
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
  GPIO_Claimed,   /*< Requested GPIO pin is already taken. */
  GPIO_Ownership, /*< GPIO pin is not claimed by provided RCC_DevId, so pin cannot be freed */
} GPIO_Err;

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
 * @fn gpio_claim_pin
 *
 * @brief Claim specific GPIO pin from a bank; This operation is usually performed by device driver. After
 *        successful invocation of this function, requested pin belongs to the device with provided
 *        rcc_devid, and it shouldn't be for anything or anyone except the entity that claimed it.
 *        If claimed pin is not needed any more, it shall be freed. Free operation is preformed with
 *        \gpio_free_pin\ function.
 *
 * @param rcc_devid: RCC Device ID of peripherial device that is trying to claim the pin. For devices
 *                   not residing within MCU, GPIO bank RCC_DevId shall be provided.
 * 
 * @param pin: Pin from GPIO bank. Check section PIN_DEFINITIONS for available options
 * 
 * @return GPIO_Err, GPIO_Success if operation was successfuly completed, error code otherwise.
 */
GPIO_Err gpio_claim_pin(RCC_DevId rcc_devid, GPIO_Pin pin);

/**
 * @fn gpio_free_pin
 *
 * @brief Free previously claimed GPIO pin.
 *
 * @param rcc_devid: RCC Device ID of peripherial device that is trying to free the pin. For devices
 *                   not residing within MCU, GPIO bank RCC_DevId shall be provided.
 *
 * @param GPIO_Err, GPIO_Success if operation was successfuly completed, error code otherwise.
 */
GPIO_Err gpio_free_pin(RCC_DevId rcc_devid, GPIO_Pin pin);

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
