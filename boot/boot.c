#include <stdarg.h>
#include <stdio.h>

#include "../src/dvp_device.h"
#include "dvp_interface.h"
#include "mmem.h"
#include "printf.h"
#include "time.h"
#include "uart_interface.h"

extern uint32_t _susrstack;

extern uint32_t _heap_end;
extern uint32_t _heap_start;

/* void uart_puts(UART_Device dev, const char *str) */
/* { */
/*   while (*str) { */
/*     uart_putc(dev, *str); */
/*     str++; */
/*   } */
/* } */

/* int _printf(const char *fmt, ...) */
/* { */
/*   va_list args; */
/*   va_start(args, fmt); */
/*   int len = vsnprintf(NULL, 0, fmt, args); */
/*   va_end(args); */

/*   char msg[len + 1]; */
/*   memset(msg, 0, len + 1); */
/*   va_start(args, fmt); */
/*   vsnprintf(msg, len + 1, fmt, args); */
/*   va_end(args); */

/*   uart_puts(UART_Device3, msg); */
/*   return len; */
/* } */

const char DVP_Err_String[][64] = {"DVP_Err_Success", "DVP_Err_ConfigFail", "DVP_Err_AlreadyEnabled",
  "DVP_Err_AlreadyConfigured", "DVP_Err_NotEnabled", "DVP_Err_NoSuchDevice", "DVP_Err_DeviceOutOfRange"};

__attribute__((section(".data"))) static uint16_t buff0[16];
__attribute__((section(".data"))) static uint16_t buff1[16];

void irq_handler_dvp(void);
int main(void)
{
  system_pll_clock_init(PLLMul_12);

  uart_enable_device(UART_Device3, UART3_DEFAULT_MAPPING);
  uart_configure_device(UART_Device3, 9600, sysclock_frequency);

  bind_printf(UART_Device3);
  DVP_Err status = dvp_enable_device(DVP_Device1, DVP_DEFAULT_MAPPING);
  if (status) {
    printf_("DVP Device enable failed. Error code: %s\r\n", DVP_Err_String[status]);
    goto _nothing;
  }

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
    .DVP_DMA_BUF0 = (uintptr_t)((uint16_t *)buff0),
    .DVP_DMA_BUF1 = (uintptr_t)((uint16_t *)buff1),
  };

  status = dvp_configure_device(DVP_Device1, dvp_config);
  if (status) {
    printf_("DVP Device configure failed. Error code: %s\r\n", DVP_Err_String[status]);
    goto _nothing;
  }

  printf_("Initialization successful\r\n");
  printf_("Syclock frequency: %d Hz\r\n", sysclock_frequency);
  printf_("dvp_config.CR0.data: 0x%02x\r\n", dvp_config.CR0.data);
  printf_("dvp_config.CR1.data: 0x%02x\r\n", dvp_config.CR1.data);
  printf_("dvp_config.IFR.data: 0x%02x\r\n", dvp_config.CR1.data);
  printf_("&buff0: 0x%08x\r\n", buff0);
  printf_("&buff1: 0x%08x\r\n", buff1);
  dvp_dump_registers(DVP_Device1);
  wait_ms(500);

_nothing:
  while (1)
    ;
}
