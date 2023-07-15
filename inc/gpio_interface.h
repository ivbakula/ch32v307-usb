#ifndef INC_GPIO_INTERFACE_H
#define INC_GPIO_INTERFACE_H
#include <stdbool.h>

typedef enum
{
  GPIO_PortA,
  GPIO_PortB,
  GPIO_PortC,
  GPIO_PortD,
  GPIO_PortE,
} GPIO_Iface;

typedef enum
{
  /* Output modes */
  GPIO_GenPushPullOut,  /*< General push-pull output mode */
  GPIO_GenOpenDrainOut, /*< General open drain output mode */
  GPIO_AltPushPullOut,  /*< Alternate function push pull output mode */
  GPIO_AltOpenDrainOut, /*< Alternate function open drain output mode */

  /* Input modes */
  GPIO_AnalogIn,   /*< Analog input mode */
  GPIO_FloatIn,    /*< Floating input mode */
  GPIO_PullUpDown, /*< Mode with pull-up and pull-down */
  GPIO_Reserved,   /*< Reserved */
} GPIO_Config;

typedef enum
{
  GPIO_ModInput,
  GPIO_ModOut10MHz,
  GPIO_ModOut2MHz,
  GPIO_ModOut50MHz,
} GPIO_Mode;
  
void gpio_pin_config(GPIO_Iface gpio_port, uint8_t pin, GPIO_Config config);

#endif
