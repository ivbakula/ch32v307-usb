#include "ch32v307.h"
#include <stddef.h>
#include <stdio.h>
#include "csr.h"
#include "time.h"
#include "irq.h"
#include "mmem.h"
#include "usb_interface.h"

extern uint32_t _susrstack;

extern uint32_t _heap_end;
extern uint32_t _heap_start;

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
  gpio_port_config(GPIOA_BASE, 9, GPIO_PUSH_PULL_ALTERNATE_OUTPUT | GPIO_OUTPUT_SPEED_10MHZ);

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

void irq_spi1(void)
{
  /* if (SPI1->STATR & U16_BIT(7)) */
  /*   uart_puts("BSY"); */

  /* if (SPI1->STATR & U16_BIT(6)) */
  /*   uart_puts("OVR"); */

  /* if (SPI1->STATR & U16_BIT(5)) */
  /*   uart_puts("MODF"); */

  /* if (SPI1->STATR & U16_BIT(4)) */
  /*   uart_puts("CRCERR"); */

  /* if (SPI1->STATR & U16_BIT(3)) */
  /*   uart_puts("UDR"); */

  /* if (SPI1->STATR & U16_BIT(2)) */
  /*   uart_puts("CHSID"); */

  /* if (SPI1->STATR & U16_BIT(1)) */
  /*   uart_puts("TXE"); */

  /* if (SPI1->STATR & U16_BIT(0)) */
  /*   uart_puts("RXNE"); */
}

void spi_dump_registers()
{
  uart_puts("===================SPI_REGISTER_DUMP_START===================\r\n");
  char buff[256] = {0};

  
  sprintf(buff, "CTRL1 [0x%08x]: 0x%04x\r\n", &SPI1->CTLR1, SPI1->CTLR1);
  uart_puts(buff);
  memset(buff, 0, 256);

  sprintf(buff, "CTRL2 [0x%08x]: 0x%04x\r\n", &SPI1->CTLR2, SPI1->CTLR2);
  uart_puts(buff);
  memset(buff, 0, 256);

  sprintf(buff, "STATR [0x%08x]: 0x%04x\r\n", &SPI1->STATR, SPI1->STATR);
  uart_puts(buff);
  memset(buff, 0, 256);
  uart_puts("===================SPI_REGISTER_DUMP_END===================\r\n");  
}

void spi_init(void)
{
  RCC->R32_RCC_APB2PCENR |= U32_BIT(12) | U32_BIT(2);
  //  RCC->R32_RCC_APB2PRSTR |= U32_BIT(12);
  
  gpio_port_config(GPIOA_BASE, 4, GPIO_PUSH_PULL_ALTERNATE_OUTPUT | GPIO_OUTPUT_SPEED_10MHZ);
  gpio_port_config(GPIOA_BASE, 5, GPIO_PUSH_PULL_ALTERNATE_OUTPUT | GPIO_OUTPUT_SPEED_10MHZ);
  gpio_port_config(GPIOA_BASE, 7, GPIO_PUSH_PULL_ALTERNATE_OUTPUT | GPIO_OUTPUT_SPEED_10MHZ);

  //  SPI1->CTLR1 = 0b100 << 3 | U16_BIT(2) | U16_BIT(8) | U16_BIT(9) | U16_BIT(6);
  //  SPI1->CTLR2 |= U16_BIT(2) | U16_BIT(5) | U16_BIT(6) | U16_BIT(7);


  SPI1->CTLR1 = 0b100 << 3;
  SPI1->CTLR1 |= U16_BIT(0) | U16_BIT(1);
  SPI1->CTLR1 |= U16_BIT(11);
  SPI1->CTLR1 |= U16_BIT(9);
  SPI1->CTLR1 |= U16_BIT(8);
  //  SPI1->CTLR2 = U16_BIT(2);
  SPI1->CTLR1 |= U16_BIT(2);
  SPI1->CTLR1 |= U16_BIT(6);

  //  wait_ms(100);
  // SPI1->CTLR1 |= U16_BIT(6);

  irq_register_interrupt_handler(SPI1_IRQn, irq_spi1);
  irq_enable_interrupt(SPI1_IRQn);
}

void spi_master_write(uint8_t byte)
{
  while (!(SPI1->STATR & U16_BIT(1)))
    ;
  SPI1->DATAR = byte;
}

void spi_master_transfer(char data[], size_t size)
{
  for (int i = 0; i < size; i++)
    spi_master_write(data[i]);
}

extern void enable_usbd(void);
extern uint8_t USBHS_EP0_Buf;
DeviceDescriptor sp = {
  .bLength            = 0x12,
  .bDescriptorType    = 0x01,
  .bcdUSB             = 0x01,
  .bDeviceClass       = 0x02,
  .bDeviceSubClass    = 0x00,
  .bDeviceProtocol    = 0x00,
  .bMaxPacketSize0    = 64,
  .idVendor           = 0x6976, /*iv*/
  .idProduct          = 0x616e, /*an*/
  .bcdDevice          = 0x01,
  .iProduct           = 0x02,
  .iSerialNumber      = 0x03,
  .bNumConfigurations = 0x01,
};

size_t usb_poll(void *buffer, uint8_t endpoint, size_t buffer_size);

typedef struct
{
  uint8_t command;
  uint8_t data[56];
} USB_Packet;

#define SPI_WRITE_COMMAND 0x1

int main()
{
  system_pll_clock_init(PLLMul_6);
  mmem_init();
  enable_uart(sysclock_frequency, 9600);
  init_usb_device(USB_FULL_SPEED);
  spi_init();
  char buffer[64];
  uart_puts("Initialization complete!\r\n");
  spi_dump_registers();
  while (1)
  {
    size_t transfer_size = 0;
    transfer_size = read_endpoint(buffer, 64, 1);
    USB_Packet *p = (USB_Packet *)buffer;
    switch (p->command)
      {
      case SPI_WRITE_COMMAND:
        uart_puts("SPI_WRITE_COMMAND\r\n");
	spi_master_transfer(p->data, strlen(p->data));
        break;
      default:
	uart_puts("UNKNOWN\r\n");
	break;
      }
  }

}    
