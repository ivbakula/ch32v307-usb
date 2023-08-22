#include "../../uart_device.h"
#include "irq.h"
#include "rcc_interface.h"
#include "uart_interface.h"

// clang-format off
const unsigned char uart_instantiated[8] = { 1, 1, 1, 0, 0, 0, 0, 0 };
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
  {
    .base = UART2_BASE,
    .rcc_devid = RCC_USART2EN,
    .irqn = USART2_IRQn,
    .enabled = false,
    .configured = false,
    .chosen_pinconfig = UART2_DEFAULT_MAPPING,
    .no_pin_configs = 2,
    .pin_configuration = {
      { PA4, PA2, PA3, PA0, PA1 },
      { PD7, PD5, PD6, PD3, PD4 },
      {},
      {}
    }
  },
  {
    .base = UART3_BASE,
    .rcc_devid = RCC_USART3EN,
    .irqn = USART3_IRQn,
    .enabled = false,
    .configured = false,
    .chosen_pinconfig = UART3_DEFAULT_MAPPING,
    .no_pin_configs = 4,
    .pin_configuration = {
      { PB12, PB10, PB11, PB13, PB14 },
      { PC12, PC10, PC11, PB13, PB14 },
      { PD10, PA13, PA14, PD11, PD12 },
      { PD10, PD8, PD9, PD11, PD12 }
    }
  },
  {
    .base = UART4_BASE,
    .rcc_devid = RCC_UART4EN,
    .irqn = UART4_IRQn,
    .enabled = false,
    .configured = false,
    .chosen_pinconfig = UART4_DEFAULT_MAPPING,
    .no_pin_configs = 2,
    .pin_configuration = {
      { },
      { PNA, PB0, PB1, PNA, PNA },
      {},
      {}
    }
  },
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
