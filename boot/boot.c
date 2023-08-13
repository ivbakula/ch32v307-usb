#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "adf435x_control.h"
#include "time.h"
#include "uart_interface.h"
#include "usb_interface.h"

void uart_puts(UART_Device dev, const char *str)
{
  while (*str) {
    uart_putc(dev, *str);
    str++;
  }
}

int _printf(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  int len = vsnprintf(NULL, 0, fmt, args);
  va_end(args);

  char msg[len + 1];
  memset(msg, 0, len + 1);
  va_start(args, fmt);
  vsnprintf(msg, len + 1, fmt, args);
  va_end(args);

  uart_puts(UART_Device1, msg);
  return len;
}

const SPI_Config spi1_config = {
  .CTRL1 =
    {
      ._SDDMEN = 1,  /* enable SDDM */
      ._SDDM_TX = 1, /* enable TX only in SDDM */
      ._CRCEN = 0,
      ._CRCNEXT = 0,
      ._DFF = 0,
      ._RXONLY = 0,
      ._NSS_CFG = 1,  /* SW NSS Control */
      ._NSS_LVL = 1,  /* NSS high */
      ._LSBFIRST = 0, /* MSB FIRST */
      ._SPI_EN = 1,   /* Enable SPI device */
      ._BAUD_RATE = SPI_CTRL1_BR_FPCLK_16,
      ._MSTR = 1,
      ._CPOL = 0,
      ._CPHA = 0,
    },
  .CTRL2 =
    {
      .data = 0,
    },
};

#define SPI_WRITE_COMMAND 0x1
#define PLL_SET_FREQUENCY 0x2

void adf435x_dump_registers(ADF435x_Regs *regs)
{
  _printf("ADF435x_REGISTER_DUMP START\r\n");
  _printf("R0: 0x%08x\r\n", regs->adf435x_regs[0]);
  _printf("R1: 0x%08x\r\n", regs->adf435x_regs[1]);
  _printf("R2: 0x%08x\r\n", regs->adf435x_regs[2]);
  _printf("R3: 0x%08x\r\n", regs->adf435x_regs[3]);
  _printf("R4: 0x%08x\r\n", regs->adf435x_regs[4]);
  _printf("R5: 0x%08x\r\n", regs->adf435x_regs[5]);
  _printf("ADF435x_REGISTER_DUMP END\r\n");
}

void handle_usb_request(uint32_t command, uint8_t *data)
{
  switch (command) {
    case SPI_WRITE_COMMAND:
      _printf("SPI_WRITE_COMMAND\r\n");
      for (int i = 0; i < strlen(data); i++) {
        spi_write(SPI_Device1, data[i]);
      }
      break;

    case PLL_SET_FREQUENCY:
      _printf("PLL_SET_FREQUENCY\r\n");
      adf435x_dump_registers((ADF435x_Regs *)data);
      adf435x_program_device(*(ADF435x_Regs *)data);
      break;
    default:
      break;
  }
}

typedef struct
{
  uint32_t command;
  uint8_t data[60];
} USB_Packet;

void usb_server(void)
{
  uart_puts(UART_Device1, "Starting USB server\r\n");
  char buffer[64];

  while (1) {
    size_t transfer_size = read_endpoint(buffer, 64, 1);
    USB_Packet *p = (USB_Packet *)buffer;
    handle_usb_request(p->command, p->data);
  }
}

const char spi_error_codes[5][32] = {
  "SPI_Err_Success",
  "SPI_Err_ConfigFail",
  "SPI_Err_AlreadyEnabled",
  "SPI_Err_NotEnabled",
  "SPI_Err_NoSuchDevice",
};

int main()
{
  system_pll_clock_init(PLLMul_6);

  uart_enable_device(UART_Device1, UART1_DEFAULT_MAPPING);
  uart_configure_device(UART_Device1, 9600, sysclock_frequency);

  uart_puts(UART_Device1, "UART Device successfuly configured\r\n");
  SPI_Err spi_err = spi_enable_device(SPI_Device1, SPI1_DEFAULT_MAPPING);
  if (spi_err) {
    _printf("SPI enable device failed for SPI_Device%d with error code: %s\r\n", SPI_Device1, spi_error_codes[spi_err]);
    goto nothing;
  }

  spi_err = spi_configure_device(SPI_Device1, spi1_config);
  if (spi_err) {
    _printf("SPI configure device failed for SPI_Device%d with error code: %s\r\n", SPI_Device1, spi_error_codes[spi_err]);
    goto nothing;
  }

  ADF435x_PinConfig pinc = {.LE = PA1, .CE = PA0, .LD = PA2};
  adf435x_create_interface(SPI_Device1, pinc);

  init_usb_device(USB_FULL_SPEED);

  _printf("Initialization complete\r\n");
  usb_server();
nothing:
  while (1)
    ;
}
