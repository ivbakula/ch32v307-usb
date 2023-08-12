#include "adf435x_control.h"
#include "gpio_interface.h"

static SPI_Device spi_iface;
static bool is_created;
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
  gpio_pin_config(pin_config.LE, GPIO_Mode_Output_50MHz, GPIO_Output_PushPullAlt);
  LE = pin_config.LE;
  
  gpio_port_enable(GET_GPIO_PORT(pin_config.CE));
  gpio_pin_config(pin_config.CE, GPIO_Mode_Output_50MHz, GPIO_Output_PushPullAlt);
  CE = pin_config.CE;
  
  gpio_port_enable(GET_GPIO_PORT(pin_config.LD));
  gpio_pin_config(pin_config.LD, GPIO_Mode_Output_50MHz, GPIO_Input_PullUpDown);
  LD = pin_config.LD;

  gpio_pin_output(CE, GPIO_HIGH);
  gpio_pin_output(LE, GPIO_HIGH);
  
  is_created = true;
  return ADF435x_Err_Success;
}

static void adf435x_write_register(uint32_t reg)
{
  
}

ADF435x_Err adf435x_program_device(ADF435x_Regs regs)
{
  if (!is_created)
    return ADF435x_Err_NoSuchDevice;

  for (int i = 0; i < 6; i++) {
    adf435x_write_register(regs.adf435x_regs[6 - 1 - i]);
  }
}  
