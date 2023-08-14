#include "../../uart_device.h"
#include "irq.h"
#include "rcc_interface.h"
#include "uart_interface.h"

// clang-format off
const unsigned char uart_instantiated[8] = { 1, 0, 0, 0, 0, 0, 0, 0 };
UART_Instance UART_Instances[8] = {
  {
    .base = UART1_BASE,
    .rcc_devid = RCC_USART1EN,
    .irqn = USART1_IRQn,
    .enabled = false,
    .configured = false,
    .chosen_pinconfig = UART1_DEFAULT_MAPPING,
    .no_pin_configs = 4,
    .pin_configuration = {
      { PA8,  PA9,  PA10, PA11, PA12 },
      { PA8,  PB6,  PB7,  PA11, PA12 },
      { PA10, PB15, PA8,  PA5,  PA9  },
      { PA5,  PA6,  PA7,  PC4,  PC5  }
    }
  },
  /* { */
  /*   .base = UART2_BASE, */
  /*   .enabled = false, */
  /*   .configured = false, */
  /*   .pin_configuration = { */
  /*     {}, */
  /*     {}, */
  /*     {}, */
  /*     {} */
  /*   } */
  /* }, */
  /* { */
  /*   .base = UART3_BASE, */
  /*   .enabled = false, */
  /*   .configured = false, */
  /*   .pin_configuration = { */
  /*     {}, */
  /*     {}, */
  /*     {}, */
  /*     {} */
  /*   } */
  /* }, */
  /* { */
  /*   .base = UART4_BASE, */
  /*   .enabled = false, */
  /*   .configured = false, */
  /*   .pin_configuration = { */
  /*     {}, */
  /*     {}, */
  /*     {}, */
  /*     {} */
  /*   } */
  /* }, */
  /* { */
  /*   .base = UART5_BASE, */
  /*   .enabled = false, */
  /*   .configured = false, */
  /*   .pin_configuration = { */
  /*     {}, */
  /*     {}, */
  /*     {}, */
  /*     {} */
  /*   } */
  /* }, */
  /* { */
  /*   .base = UART6_BASE, */
  /*   .enabled = false, */
  /*   .configured = false, */
  /*   .pin_configuration = { */
  /*     {}, */
  /*     {}, */
  /*     {}, */
  /*     {} */
  /*   } */
  /* }, */
  /* { */
  /*   .base = UART7_BASE, */
  /*   .enabled = false, */
  /*   .configured = false, */
  /*   .pin_configuration = { */
  /*     {}, */
  /*     {}, */
  /*     {}, */
  /*     {} */
  /*   } */
  /* }, */
  /* { */
  /*   .base = UART8_BASE, */
  /*   .enabled = false, */
  /*   .configured = false, */
  /*   .pin_configuration = { */
  /*     {}, */
  /*     {}, */
  /*     {}, */
  /*     {} */
  /*   } */
  /* } */
};
