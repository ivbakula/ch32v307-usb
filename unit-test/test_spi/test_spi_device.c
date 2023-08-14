#include <string.h>
#include <unity.h>

#include "Mockgpio_interface.h"
#include "Mockmmio_ops.h"
#include "Mockrcc_interface.h"
#include "gpio_interface.h"
#include "rcc_interface.h"
#include "spi_device.h"
#include "spi_interface.h"

// clang-format off
const bool spi_instantiated[3] = { 1, 0, 0 };
SPI_Instance SPI_Instances[3] = {
  {
    .base = SPI1_BASE,
    .rcc_devid = RCC_SPI1EN,
    .irqn = SPI1_IRQn,
    .enabled = false,
    .configured = false,
    .chosen_pinconfig = SPI1_DEFAULT_MAPPING,
    .no_pin_configs = SPI1_NO_OF_PINCONFIGS,
    .pin_configuration = {
      /*NSS  SCK MISO  MOSI */
      { PA4, PA5, PA6, PA7 }, /* Default configuration */
      { PA15, PB3, PB4, PB5 } /* Alternate configuration */
    },
  },
};
// clang-format on

static const GPIO_Pin pin_configurations_spi1[2][4] = {
  {PA4, PA5, PA6, PA7}, /* Default configuration */
  {PA15, PB3, PB4, PB5} /* Alternate configuration */
};

const SPI_Config spi_config = {
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

const uint32_t SPI_CTRL1_REGISTER =
  U16_BIT(15) | U16_BIT(14) | U16_BIT(9) | U16_BIT(8) | U16_BIT(6) | U16_BIT(4) | U16_BIT(3) | U16_BIT(2);

void setUp(void)
{
  SPI_Instances[0].base = SPI1_BASE;
  SPI_Instances[0].rcc_devid = RCC_SPI1EN;
  SPI_Instances[0].irqn = SPI1_IRQn;
  SPI_Instances[0].enabled = false;
  SPI_Instances[0].configured = false;
  SPI_Instances[0].chosen_pinconfig = SPI1_DEFAULT_MAPPING;
  SPI_Instances[0].no_pin_configs = SPI1_NO_OF_PINCONFIGS;
  memcpy(&SPI_Instances[0].pin_configuration[0][0], &pin_configurations_spi1[0][0], sizeof(pin_configurations_spi1));
}

void tearDown(void)
{
  memset(&SPI_Instances[0], 0, sizeof(SPI_Instances));
}

void test_spi_enable_no_such_device_failure(void)
{
  TEST_ASSERT_EQUAL(SPI_Err_NoSuchDevice, spi_enable_device(SPI_Device2, SPI1_DEFAULT_MAPPING));
}

void test_spi_enable_already_enabled_failure(void)
{
  SPI_Instances[0].enabled = true;
  TEST_ASSERT_EQUAL(SPI_Err_AlreadyEnabled, spi_enable_device(SPI_Device1, SPI1_DEFAULT_MAPPING));
}

void test_spi_enable_invalid_pinconfig_failure(void)
{
  TEST_ASSERT_EQUAL(SPI_Err_ConfigFail, spi_enable_device(SPI_Device1, 115));
}

void test_spi_enable_config_failure_locked_pin(void)
{
  rcc_pcendis_Expect(RCC_SPI1EN, RCC_ENABLE);
  gpio_port_enable_Expect(GET_GPIO_PORT(PA4));
  gpio_lock_pin_ExpectAndReturn(RCC_SPI1EN, PA4, GPIO_Success);

  gpio_port_enable_Expect(GET_GPIO_PORT(PA5));
  gpio_lock_pin_ExpectAndReturn(RCC_SPI1EN, PA5, GPIO_Success);

  gpio_port_enable_Expect(GET_GPIO_PORT(PA6));
  gpio_lock_pin_ExpectAndReturn(RCC_SPI1EN, PA6, GPIO_Success);

  gpio_port_enable_Expect(GET_GPIO_PORT(PA7));
  gpio_lock_pin_ExpectAndReturn(RCC_SPI1EN, PA7, GPIO_Ownership);

  gpio_unlock_pin_ExpectAndReturn(RCC_SPI1EN, PA4, GPIO_Success);
  gpio_unlock_pin_ExpectAndReturn(RCC_SPI1EN, PA5, GPIO_Success);
  gpio_unlock_pin_ExpectAndReturn(RCC_SPI1EN, PA6, GPIO_Success);
  gpio_unlock_pin_ExpectAndReturn(RCC_SPI1EN, PA7, GPIO_Success);

  TEST_ASSERT_EQUAL(SPI_Err_ConfigFail, spi_enable_device(SPI_Device1, SPI1_DEFAULT_MAPPING));
}

void test_spi_enable_success(void)
{
  rcc_pcendis_Expect(RCC_SPI1EN, RCC_ENABLE);
  gpio_port_enable_Expect(GET_GPIO_PORT(PA4));
  gpio_lock_pin_ExpectAndReturn(RCC_SPI1EN, PA4, GPIO_Success);
  gpio_port_enable_Expect(GET_GPIO_PORT(PA5));
  gpio_lock_pin_ExpectAndReturn(RCC_SPI1EN, PA5, GPIO_Success);
  gpio_port_enable_Expect(GET_GPIO_PORT(PA6));
  gpio_lock_pin_ExpectAndReturn(RCC_SPI1EN, PA6, GPIO_Success);
  gpio_port_enable_Expect(GET_GPIO_PORT(PA7));
  gpio_lock_pin_ExpectAndReturn(RCC_SPI1EN, PA7, GPIO_Success);

  TEST_ASSERT_EQUAL(SPI_Err_Success, spi_enable_device(SPI_Device1, SPI1_DEFAULT_MAPPING));
  TEST_ASSERT_EQUAL(SPI_Instances[0].chosen_pinconfig, SPI1_DEFAULT_MAPPING);
  TEST_ASSERT_TRUE(SPI_Instances[0].enabled);
}

void test_spi_disable_no_such_device_fail(void)
{
  TEST_ASSERT_EQUAL(SPI_Err_NoSuchDevice, spi_disable_device(SPI_Device3));
}

void test_spi_disable_not_enabled_fail(void)
{
  TEST_ASSERT_EQUAL(SPI_Err_NotEnabled, spi_disable_device(SPI_Device1));
}

void test_spi_disable_success(void)
{
  SPI_Instances[0].enabled = true;

  rcc_pcendis_Expect(RCC_SPI1EN, RCC_DISABLE);
  gpio_unlock_pin_ExpectAndReturn(RCC_SPI1EN, PA4, GPIO_Success);
  gpio_unlock_pin_ExpectAndReturn(RCC_SPI1EN, PA5, GPIO_Success);
  gpio_unlock_pin_ExpectAndReturn(RCC_SPI1EN, PA6, GPIO_Success);
  gpio_unlock_pin_ExpectAndReturn(RCC_SPI1EN, PA7, GPIO_Success);

  TEST_ASSERT_EQUAL(SPI_Err_Success, spi_disable_device(SPI_Device1));
  TEST_ASSERT_FALSE(SPI_Instances[0].enabled);
}

void test_configure_device_no_such_device_fail(void)
{
  TEST_ASSERT_EQUAL(SPI_Err_NoSuchDevice, spi_configure_device(SPI_Device3, spi_config));
}

void test_configure_device_not_enabled_fail(void)
{
  TEST_ASSERT_EQUAL(SPI_Err_NotEnabled, spi_configure_device(SPI_Device1, spi_config));
}

void test_configure_device_success(void)
{
  SPI_Instances[0].enabled = true;
  gpio_pin_config_Expect(PA4, GPIO_Mode_Output_50MHz, GPIO_Output_PushPullAlt);
  gpio_pin_config_Expect(PA5, GPIO_Mode_Output_50MHz, GPIO_Output_PushPullAlt);
  gpio_pin_config_Expect(PA6, GPIO_Mode_Output_50MHz, GPIO_Output_PushPullAlt);
  gpio_pin_config_Expect(PA7, GPIO_Mode_Output_50MHz, GPIO_Output_PushPullAlt);
  gpio_unlock_pin_ExpectAndReturn(RCC_SPI1EN, PA6, GPIO_Success);

  mmio_writew_Expect(_SPI_REGISTER(SPI_Instances[0].base, CTRL1), SPI_CTRL1_REGISTER);
  mmio_writew_Expect(_SPI_REGISTER(SPI_Instances[0].base, CTRL2), 0);

  TEST_ASSERT_EQUAL(SPI_Err_Success, spi_configure_device(SPI_Device1, spi_config));

  TEST_ASSERT_EQUAL(SPI_Instances[0].packet_sz, SPI_PACKET_TRANSFER_SIZE_8);
  TEST_ASSERT_TRUE(SPI_Instances[0].configured);
}

void test_uart_reset_device(void)
{
  // TODO depends on implementing reset infrastructure in RCC subsystem
}

void test_spi_write_u8(void)
{
  const uint8_t data = 'U';
  SPI_Instances[0].packet_sz = SPI_PACKET_TRANSFER_SIZE_8;
  SPI_Instances[0].enabled = true;
  SPI_Instances[0].configured = true;
  mmio_and_readw_ExpectAndReturn(_SPI_REGISTER(SPI_Instances[0].base, STATR), U16_BIT(1), U16_BIT(1));

  mmio_writew_Expect(_SPI_REGISTER(SPI_Instances[0].base, DATAR), data);
  spi_write(SPI_Device1, data);
}

void test_spi_read_u8(void)
{
  const uint8_t data = 0xde;
  SPI_Instances[0].packet_sz = SPI_PACKET_TRANSFER_SIZE_8;
  SPI_Instances[0].enabled = true;
  SPI_Instances[0].configured = true;
  mmio_and_readw_ExpectAndReturn(_SPI_REGISTER(SPI_Instances[0].base, STATR), U16_BIT(0), U16_BIT(0));
  mmio_and_readb_ExpectAndReturn(_SPI_REGISTER(SPI_Instances[0].base, DATAR), 0xff, data);
  TEST_ASSERT_EQUAL(data, spi_read(SPI_Device1));
}

void test_spi_write_u16(void)
{
  const uint16_t data = 0xdead;
  SPI_Instances[0].packet_sz = SPI_PACKET_TRANSFER_SIZE_16;
  SPI_Instances[0].enabled = true;
  SPI_Instances[0].configured = true;
  mmio_and_readw_ExpectAndReturn(_SPI_REGISTER(SPI_Instances[0].base, STATR), U16_BIT(1), U16_BIT(1));
  mmio_writew_Expect(_SPI_REGISTER(SPI_Instances[0].base, DATAR), data);
  spi_write(SPI_Device1, data);
}

void test_spi_read_u16(void)
{
  const uint16_t data = 0xdead;
  SPI_Instances[0].packet_sz = SPI_PACKET_TRANSFER_SIZE_16;
  SPI_Instances[0].enabled = true;
  SPI_Instances[0].configured = true;
  mmio_and_readw_ExpectAndReturn(_SPI_REGISTER(SPI_Instances[0].base, STATR), U16_BIT(0), U16_BIT(0));
  mmio_readw_ExpectAndReturn(_SPI_REGISTER(SPI_Instances[0].base, DATAR), data);
  TEST_ASSERT_EQUAL(data, spi_read(SPI_Device1));
}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_spi_enable_no_such_device_failure);
  RUN_TEST(test_spi_enable_already_enabled_failure);
  RUN_TEST(test_spi_enable_invalid_pinconfig_failure);
  RUN_TEST(test_spi_enable_config_failure_locked_pin);
  RUN_TEST(test_spi_enable_success);

  RUN_TEST(test_spi_disable_no_such_device_fail);
  RUN_TEST(test_spi_disable_not_enabled_fail);
  RUN_TEST(test_spi_disable_success);

  RUN_TEST(test_configure_device_no_such_device_fail);
  RUN_TEST(test_configure_device_not_enabled_fail);
  RUN_TEST(test_configure_device_success);

  RUN_TEST(test_spi_read_u8);
  RUN_TEST(test_spi_read_u16);
  RUN_TEST(test_spi_write_u8);
  RUN_TEST(test_spi_write_u16);

  return UNITY_END();
}
