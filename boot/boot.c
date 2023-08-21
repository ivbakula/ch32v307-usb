#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/dvp_device.h"
#include "adf435x_interface.h"
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

  uart_puts(UART_Device3, msg);
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
      adf435x_program_device(ADF435x_Device1, *(ADF435x_Regs *)data);
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

const char adf435x_error_codes[5][64] = {
  "ADF435x_Err_Success",
  "ADF435x_Err_ConfigFail,"
  "ADF435x_Err_NoSuchDevice",
  "ADF435x_Err_NotBinded",
  "ADF435x_Err_NotEnabled",
};

/*
1) Clear RB_DVP_ALL_CLR and RB_DVP_RCV_CLR using the R8_DVP_CR1 register.

2) Configure the image mode, data bit width, PCLK polarity, HSYNC polarity and VSYNC polarity through
the R8_DVP_CR0 register to match the output of the sensor.

3) According to the effective image pixels output by the configured image sensor, configure the
R16_DVP_ROW_NUM and R16_DVP_COL_NUM registers to match the output of the sensor. In the
image JPEG mode, only configure the R16_DVP_COL_NUM register.

4) Configure the DMA receive address through the R32_DVP_DMA_BUF0/1 register.

5) If the snapshot mode is used, the RB_DVP_CM field needs to be configured through the R8_DVP_CR1
register to enable the snapshot mode.

6) If the cropping mode is used, the RB_DVP_CROP and RB_DVP_FCRC fields need to be configured
through the R8_DVP_CR1 register to enable the cropping function and control the frame capture rate. At
the same time, configure the R16_DVP_HOFFCNT, R16_DVP_VST, R16_DVP_CAPCNT and R16_DVP_VLINE to set the size of the cropped
image.

7) According to the requirements, enable the corresponding interrupt through the R8_DVP_IER register,
configure the interrupt priority through the interrupt controller NVIC or PFIC, and enable the DVP
interrupt.

8) The DMA is enabled through the R8_DVP_CR1 register, and the DVP interface is enabled through the
R8_DVP_CR0 register.

9) Wait for the generation of the relevant receiving interrupt, and process the received data in time.
 */

__attribute__((section(".data"))) uint16_t buff0[64] = {};
__attribute__((section(".data"))) uint16_t buff1[64] = {};

void irq_dvp(void)
{
  _printf("0x%02x\r\n", DVP->R8_DVP_IFR);
  if (DVP->R8_DVP_IFR & U8_BIT(0)) {
    _printf("START_FRAME_INTERRUPT\r\n");
    DVP->R8_DVP_IFR &= ~U8_BIT(0);
  }

  if (DVP->R8_DVP_IFR & U8_BIT(1)) {
    _printf("ROW_DONE_INTERRUPT\r\n");
    /* ROW done */
    uint8_t *bytes = (uint8_t *)buff0;
    DVP->R8_DVP_IFR &= ~U8_BIT(1);
    //    DVP->R8_DVP_CR0 = 0;
    for (int i = 0; i < 10; i++) {
      _printf("buff0[%d]: 0x%04x\r\n", i, buff0[i]);
      //      _printf("0x%02x %02x\r\n", bytes[i + 1], bytes);
    }
  }

  if (DVP->R8_DVP_IFR & U8_BIT(2)) {
    _printf("FRAME_DONE_INTERRUPT\r\n");
    DVP->R8_DVP_IFR &= ~U8_BIT(2);
  }

  if (DVP->R8_DVP_IFR & U8_BIT(3)) {
    _printf("FIFO_OVERFLOW_INTERRUPT\r\n");
    DVP->R8_DVP_IFR &= ~U8_BIT(3);
  }

  if (DVP->R8_DVP_IFR & U8_BIT(4)) {
    _printf("FRAME_STOP_INTERRUPT\r\n");
    DVP->R8_DVP_IFR &= ~U8_BIT(4);
  }

  return;
}

int main()
{
  system_pll_clock_init(PLLMul_12);

  uart_enable_device(UART_Device3, UART3_DEFAULT_MAPPING);
  uart_configure_device(UART_Device3, 9600, sysclock_frequency);

  GPIO_Pin dvp_pins[16] = {PA4, PA5, PA6, PA9, PA10, PB3, PB6, PB8, PB9, PC8, PC9, PC10, PC11, PC12, PD2, PD6};

  rcc_pcendis(RCC_DVPEN, RCC_ENABLE);
  for (int i = 0; i < 16; i++) {
    if (gpio_lock_pin(RCC_DVPEN, dvp_pins[i])) {
      _printf("Unable to lock pin\r\n");
      goto nothing;
    }
    gpio_port_enable(GET_GPIO_PORT(dvp_pins[i]));
    gpio_pin_config(dvp_pins[i], GPIO_Mode_Input, GPIO_Input_PullUpDown);
    gpio_pin_pullstate_config(dvp_pins[i], GPIO_PullState_Up);
  }

  DVP->R8_DVP_CR0 &= ~0x30;
  DVP->R8_DVP_CR1 &= ~(U8_BIT(1) | U8_BIT(2)); /* clear RB_DVP_ALL_CLR and RB_DVP_RCV_CLR using the R8_DVP_CR1 */
  DVP->R8_DVP_CR0 =
    /*U8_BIT(6) |*/ U8_BIT(5); /* RAW image mode, 12 bit data width, PCLK posedge, HSYNC neg polarity VSYNC neg polarity */
  DVP->R8_DVP_CR1 |= U8_BIT(4);
  DVP->R16_DVP_COL_NUM = 10;
  DVP->R16_DVP_ROW_NUM = 1;
  DVP->R32_DVP_DMA_BUF0 = (uintptr_t)((uint16_t *)buff0);
  DVP->R32_DVP_DMA_BUF1 = (uintptr_t)((uint16_t *)buff1);
  DVP->R8_DVP_IER = 0xff;
  irq_register_interrupt_handler(DVP_IRQn, irq_dvp);
  irq_enable_interrupt(DVP_IRQn);
  DVP->R8_DVP_CR1 |= U8_BIT(0);
  DVP->R8_DVP_CR0 |= U8_BIT(0);

  _printf("Initialization complete\r\n");
  _printf("Sysclock frequency: %d Hz\r\n", sysclock_frequency);

  while (!buff1[0])
    ;

  _printf("DMA0[0] = %x\r\n", buff0[0]);
  _printf("DMA1[0] = %x\r\n", buff1[0]);
nothing:
  while (1)
    ;
}
