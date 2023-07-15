#ifndef INC_UART_INTERFACE_H
#define INC_UART_INTERFACE_H

typedef struct
{
  uintptr_t mmio_base;
  uint32_t rcc_devid;
  uint8_t gpio_pins[];
} USART_Instance;

typedef enum
{
  USART1_Iface,
  USART2_Iface,
  USART3_Iface,
  UART4_Iface,
  UART5_Iface,
  UART6_Iface,
  UART7_Iface,
  UART8_Iface,
  USART_MAX
} USART_Iface;

void usart_init(USART_Iface iface, uint32_t apbclk_freq, uint16_t baud);

USART_Instance *usart_create_instance(USART_Iface iface, uint32_t apbclk_freq, uint16_t baud);
void usart_putc(USART_Iface iface, char c);

char usart_getc(USART_Iface iface);

#endif
