#include "ch32v307.h"
#include <stddef.h>
#include <stdio.h>
#include "csr.h"
#include "time.h"
#include "irq.h"

void gpio_port_config(uint32_t gpio_base, uint8_t port, uint8_t cfg)
{
  uint8_t shift = 0;
  uint32_t *reg = 0;

  if (port > 15)
    return;
  
  if (port < 8) {
    shift = port << 2;
    reg = (uint32_t *)(gpio_base + offsetof(GPIO_Regfile, R32_GPIO_CFGLR));
  } else {
    shift = (port - 8) << 2;
    reg = (uint32_t *)(gpio_base + offsetof(GPIO_Regfile, R32_GPIO_CFGHR));
  }

  *reg &= ~(0xf << shift);
  *reg |= (0xf & cfg) << shift;
}

uint16_t calculate_brr(uint32_t apb_clock, uint32_t uart_baud_rate)
{
  uint16_t over8div = 4;
  uint16_t integer_divider = (25 * (apb_clock)) / ((over8div) * (uart_baud_rate));
  uint16_t fractional_divider = integer_divider % 100;

  return ((((integer_divider) / 100) << 4) | (((((fractional_divider) * ((over8div)*2)) + 50)/100)&7));
}

void irq_uart_rx(void)
{
  char c = 0;
  if (USART1->R32_USART_STATR & U32_BIT(5))
    c = USART1->R32_USART_DATAR;
  
  return;
}

void enable_uart(uint32_t apbclk, uint16_t baud)
{
  RCC->R32_RCC_APB2PCENR |= RCC_APB2PCENR_IOPAEN;
  RCC->R32_RCC_APB2PCENR |= RCC_APB2PCENR_USART1EN;
  
  gpio_port_config(GPIOA_BASE, 10, GPIO_PULL_UP_INPUT);
  gpio_port_config(GPIOA_BASE, 9, GPIO_PUSH_PULL_ALTERNATE_OUTPUT);

  USART1->R32_USART_CTRL1 = USART_CTRL1_TE | USART_CTRL1_RE;
  USART1->R32_USART_CTRL2 = 0;
  USART1->R32_USART_CTRL3 = 0;
  USART1->R32_USART_BRR = calculate_brr(apbclk, baud);

  USART1->R32_USART_CTRL1 |= USART_CTRL1_RXNEIE | USART_CTRL1_UE;
  irq_register_interrupt_handler(USART1_IRQn, irq_uart_rx);
  irq_enable_interrupt(USART1_IRQn);
}

void uart_putc(char c)
{
  while (!(USART1->R32_USART_STATR & USART_STATR_TC))
    ;

  USART1->R32_USART_DATAR = c;
}

void uart_puts(const char *str)
{
  while (*str) {
    uart_putc(*str);
    str++;
  }
}

void red_light(void)
{
  RCC->R32_RCC_APB2PCENR |= RCC_APB2PCENR_IOPAEN;
  gpio_port_config(GPIOA_BASE, 15, 0b0001);
  GPIOA->R32_GPIO_OUTDR |= 0b1 << 15;
}

extern void enable_usbd(void);

int main()
{
  //  config_clksrc_hse_pll();
  system_pll_clock_init(PLLMul_6);
  enable_uart(sysclock_frequency, 9600);
  enable_usbd();
  irq_enable_interrupt(USBHS_IRQn);

  char st[32] = {0};
  sprintf(st, "irq status %d\r\n", irq_get_interrupt_status(USBHS_IRQn));
  uart_puts(st);

  uint32_t secs = 0;
  char buff[32] = {0};
  while (1)
    ;
}    
