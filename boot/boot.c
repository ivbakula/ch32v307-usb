#include <stdarg.h>
#include <stdio.h>

#include "../src/dvp_device.h"
#include "adf435x_interface.h"
#include "dvp_interface.h"
#include "mmem.h"
#include "printf.h"
#include "spi_interface.h"
#include "time.h"
#include "uart_interface.h"
#include "usb_interface.h"

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

DVP_Config dvp_config = {
  .CR0 = {.DVP_ENABLE = 1,
    .DVP_VSYNC_POL = DVP_VSYNC_POL_LOW,
    .DVP_HSYNC_POL = DVP_HSYNC_POL_HIGH,
    .DVP_PCLK_POL = DVP_PCLK_POL_POSEDGE,
    .DVP_DATA_WIDTH = DVP_BusWidth_12bit,
    .DVP_MODE = DVP_DATA_FORMAT_RAW},
  .CR1 =
    {
      .DVP_DMA_ENABLE = DVP_ENABLE_DMA,
      .DVP_ALL_CLR = 0,
      .DVP_RCV_CLR = 0,
      .DVP_BUF_TOG = 0,
      .DVP_CAPTURE_MODE = DVP_CAPTURE_MODE_SNAPSHOT,
      .DVP_CROP = DVP_CROP_DISABLED,
      .DVP_FCRC = DVP_FCRC_ALL_FRAMES,
    },
  .IER =
    {
      .DVP_FRM_STRT = DVP_FRAME_START_INTERRUPT_EN,
      .DVP_ROW_DONE = DVP_ROW_DONE_INTERRUPT_EN,
      .DVP_FRM_DONE = DVP_FRM_DONE_INTERRUPT_EN,
      .DVP_FIFO_OV = DVP_FIFO_OV_INTERRUPT_EN,
      .DVP_STP_FRM = DVP_STP_FRM_INTERRUPT_EN,
    },

  .DVP_ROW_NUM = 1,
  .DVP_COL_NUM = 16,
  //    .DVP_DMA_BUF0 = (uintptr_t)((uint16_t *)buff0),
  //    .DVP_DMA_BUF1 = (uintptr_t)((uint16_t *)buff1),
};

#define SPI_WRITE_COMMAND 0x1
#define PLL_SET_FREQUENCY 0x2

void adf435x_dump_registers(ADF435x_Regs *regs)
{
  printf_("ADF435x_REGISTER_DUMP START\r\n");
  printf_("R0: 0x%08x\r\n", regs->adf435x_regs[0]);
  printf_("R1: 0x%08x\r\n", regs->adf435x_regs[1]);
  printf_("R2: 0x%08x\r\n", regs->adf435x_regs[2]);
  printf_("R3: 0x%08x\r\n", regs->adf435x_regs[3]);
  printf_("R4: 0x%08x\r\n", regs->adf435x_regs[4]);
  printf_("R5: 0x%08x\r\n", regs->adf435x_regs[5]);
  printf_("ADF435x_REGISTER_DUMP END\r\n");
}

void handle_usb_request(uint32_t command, uint8_t *data)
{
  switch (command) {
    case SPI_WRITE_COMMAND:
      printf_("SPI_WRITE_COMMAND\r\n");
      for (int i = 0; i < strlen(data); i++) {
        spi_write(SPI_Device1, data[i]);
      }
      break;

    case PLL_SET_FREQUENCY:
      printf_("PLL_SET_FREQUENCY\r\n");
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
  uart_puts(UART_Device3, "Starting USB server\r\n");
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

const char DVP_Err_String[][64] = {
  "DVP_Err_Success",
  "DVP_Err_ConfigFail",
  "DVP_Err_AlreadyEnabled",
  "DVP_Err_AlreadyConfigured",
  "DVP_Err_NotEnabled",
  "DVP_Err_NoSuchDevice",
  "DVP_Err_DeviceOutOfRange",
};

const char ADF435x_Err_String[][64] = {
  "ADF435x_Err_Success",
  "ADF435x_Err_ConfigFail",
  "ADF435x_Err_NoSuchDevice",
  "ADF435x_Err_NotBinded",
  "ADF435x_Err_NotEnabled",
};

__attribute__((section(".data"))) static uint16_t buff0[16];
__attribute__((section(".data"))) static uint16_t buff1[16];

void irq_handler_dvp(void);
int main(void)
{
  system_pll_clock_init(PLLMul_12);

  uart_enable_device(UART_Device3, UART3_DEFAULT_MAPPING);
  uart_configure_device(UART_Device3, 9600, sysclock_frequency);
  bindprintf_(UART_Device3);

  printf_("UART Device%d successfuly initialized\r\n", UART_Device3);

  SPI_Err spi_err = spi_enable_device(SPI_Device2, SPI1_DEFAULT_MAPPING);
  if (spi_err) {
    printf_("SPI enable device failed for SPI_Device%d with error code: %s\r\n", SPI_Device2, spi_error_codes[spi_err]);
    goto nothing;
  }

  init_usb_device(USB_FULL_SPEED);

  spi_err = spi_configure_device(SPI_Device2, spi1_config);
  if (spi_err) {
    printf_("SPI configure device failed for SPI_Device%d with error code: %s\r\n", SPI_Device2, spi_error_codes[spi_err]);
    goto nothing;
  }

  ADF435x_Err adf435x_err = adf435x_bind_device(ADF435x_Device1, SPI_Device2, ADF435x_PinConfig_Default);
  if (adf435x_err) {
    printf_("ADF435x device binding failed for ADF435x Device%d with error code: %s\r\n", ADF435x_Device1,
      ADF435x_Err_String[adf435x_err]);
    goto nothing;
  }

  adf435x_err = adf435x_enable_device(ADF435x_Device1);
  if (adf435x_err) {
    printf_("ADF435x device enable failed for ADF435x_Device%d with error code: %s\r\n", ADF435x_Device1,
      ADF435x_Err_String[adf435x_err]);
    goto nothing;
  }

  DVP_Err status = dvp_enable_device(DVP_Device1, DVP_DEFAULT_MAPPING);
  if (status) {
    printf_("DVP Device enable failed. Error code: %s\r\n", DVP_Err_String[status]);
    goto nothing;
  }

  dvp_config.DVP_DMA_BUF0 = (uintptr_t)((uint16_t *)buff0);
  dvp_config.DVP_DMA_BUF1 = (uintptr_t)((uint16_t *)buff1);

  status = dvp_configure_device(DVP_Device1, dvp_config);
  if (status) {
    printf_("DVP Device configure failed. Error code: %s\r\n", DVP_Err_String[status]);
    goto nothing;
  }

  printf_("Initialization successful\r\n");
  printf_("Syclock frequency: %d Hz\r\n", sysclock_frequency);

  usb_server();
nothing:
  while (1)
    ;
}
