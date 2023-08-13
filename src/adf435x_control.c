#include "adf435x_control.h"

#include <stddef.h>

#include "time.h"

static SPI_Device spi_iface;
static bool is_created;
static size_t spi_transfer_size;
static GPIO_Pin LE;
static GPIO_Pin CE;
static GPIO_Pin LD;

ADF435x_Err adf435x_create_interface(SPI_Device spi_bus, ADF435x_PinConfig pin_config)
{
  if (!spi_is_enabled(spi_bus))
    return ADF435x_Err_ConfigFail;

  if (!spi_is_configured(spi_bus))
    return ADF435x_Err_ConfigFail;

  spi_iface = spi_bus;

  // TODO lock GPIOs. This one depends on Issue: Implement better pin locking #30
  gpio_port_enable(GET_GPIO_PORT(pin_config.LE));
  gpio_pin_config(pin_config.LE, GPIO_Mode_Output_50MHz, GPIO_Output_PushPull);
  LE = pin_config.LE;

  gpio_port_enable(GET_GPIO_PORT(pin_config.CE));
  gpio_pin_config(pin_config.CE, GPIO_Mode_Output_50MHz, GPIO_Output_PushPull);
  CE = pin_config.CE;

  gpio_port_enable(GET_GPIO_PORT(pin_config.LD));
  gpio_pin_config(pin_config.LD, GPIO_Mode_Output_50MHz, GPIO_Input_PullUpDown);
  LD = pin_config.LD;

  gpio_pin_output(CE, GPIO_HIGH);
  gpio_pin_output(LE, GPIO_HIGH);

  spi_transfer_size = spi_packet_size(spi_iface);
  is_created = true;
  return ADF435x_Err_Success;
}

typedef union
{
  uint8_t r8[4];   /* use this array if spi_transfer_size == SPI_PACKET_TRANSFER_SIZE_8 */
  uint16_t r16[2]; /* use this array if spi_transfer_size == SPI_PACKET_TRANSFER_SIZE_16 */

  uint32_t reg; /*  Whole register */
} ADF435x_RegisterHelper;

static void adf435x_write_register(uint32_t reg)
{
  ADF435x_RegisterHelper helper = {.reg = reg};

  gpio_pin_output(LE, GPIO_LOW);
  for (int i = 0; i < 4; i++) {
    spi_write(spi_iface, helper.r8[3 - i]);
  }

  wait_us(5);
  gpio_pin_output(LE, GPIO_HIGH);
}

ADF435x_Err adf435x_program_device(ADF435x_Regs regs)
{
  if (!is_created)
    return ADF435x_Err_NoSuchDevice;

  for (int i = 0; i < 6; i++) {
    adf435x_write_register(regs.adf435x_regs[6 - 1 - i]);
  }

  while (gpio_pin_input(LD))
    ;
  return ADF435x_Err_Success;
}
