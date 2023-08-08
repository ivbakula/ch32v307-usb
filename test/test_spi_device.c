#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <unity.h>

#include "gpio_device.h"
#include "rcc_device.h"
#include "spi_device.h"

void setUp(void)
{
}

void tearDown(void)
{
  memset(&spi1_regfile, 0, sizeof(spi1_regfile));
}

void test_spi_enable_disable_device_spi1_1line_tx_config(void)
{
  const uint32_t push_pull_alternate_output_50MHz_PA4 = (0b1011 << 16);
  const uint32_t push_pull_alternate_output_50MHz_PA5 = (0b1011 << 20);
  const uint32_t push_pull_alternate_output_50MHz_PA7 = (0b1011 << 28);

  const uint32_t pa4_mask = (0xf << 16);
  const uint32_t pa5_mask = (0xf << 20);
  const uint32_t pa7_mask = (0xf << 28);

  /* Lock SCK pin and fail with ConfigFail error code */
  gpio_lock_pin(1, PA4);
  TEST_ASSERT_EQUAL(SPI_Err_ConfigFail, spi_enable_device(SPI_Device1, SPI1_DEFAULT_MAPPING));

  /* Unlock TX pin and enable SPI successfuly */
  gpio_unlock_pin(1, PA4);
  TEST_ASSERT_EQUAL(SPI_Err_Success, spi_enable_device(SPI_Device1, SPI1_DEFAULT_MAPPING));
  TEST_ASSERT_EQUAL(U32_BIT(12) | U32_BIT(2), rcc_regfile.R32_RCC_APB2PCENR);
  TEST_ASSERT_EQUAL(push_pull_alternate_output_50MHz_PA4, gpioa_regfile.R32_GPIO_CFGLR & pa4_mask);
  TEST_ASSERT_EQUAL(push_pull_alternate_output_50MHz_PA5, gpioa_regfile.R32_GPIO_CFGLR & pa5_mask);
  TEST_ASSERT_EQUAL(push_pull_alternate_output_50MHz_PA7, gpioa_regfile.R32_GPIO_CFGLR & pa7_mask);

  /* Fail with AlreadyEnabled error code */
  TEST_ASSERT_EQUAL(SPI_Err_AlreadyEnabled, spi_enable_device(SPI_Device1, SPI1_DEFAULT_MAPPING));

  /* Disable successfuly */
  TEST_ASSERT_EQUAL(SPI_Err_Success, spi_disable_device(SPI_Device1));
  TEST_ASSERT_EQUAL(U32_BIT(2), rcc_regfile.R32_RCC_APB2PCENR);

  /* Fail with NotEnabled error code */
  TEST_ASSERT_EQUAL(SPI_Err_NotEnabled, spi_disable_device(SPI_Device1));
}

void test_spi_configure_device(void)
{
  const uint32_t cfgr1 =
    U16_BIT(15) | U16_BIT(14) | U16_BIT(9) | U16_BIT(8) | U16_BIT(6) | U16_BIT(4) | U16_BIT(3) | U16_BIT(2);

  const SPI_Config spi_config = {
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
    ._BAUD_RATE = SPI_CTRL1_BR_FPCLK_128,
    ._MSTR = 1,
    ._CPOL = 0,
    ._CPHA = 0,
  };

  TEST_ASSERT_EQUAL(cfgr1, spi_config.cfgr1);

  /* Fail with NotEnabled error code */
  TEST_ASSERT_EQUAL(SPI_Err_NotEnabled, spi_configure_device(SPI_Device1, spi_config));

  /* Pass */
  TEST_ASSERT_EQUAL(SPI_Err_Success, spi_enable_device(SPI_Device1, SPI1_DEFAULT_MAPPING));
  TEST_ASSERT_EQUAL(SPI_Err_Success, spi_configure_device(SPI_Device1, spi_config));

  TEST_ASSERT_EQUAL(cfgr1, spi1_regfile.CTRL1);
  TEST_ASSERT_EQUAL(0, spi1_regfile.CTRL2);
}

void test_spi_reset_device(void)
{
}

void *test_spi_write_helper(void *)
{
  sleep(1);
  spi1_regfile.STATR |= U16_BIT(1);
}

void test_spi_write_u8(void)
{
  pthread_t helper_thread;
  pthread_create(&helper_thread, NULL, test_spi_write_helper, NULL);
  spi_write_u8(SPI_Device1, 'U');
  pthread_join(helper_thread, NULL);

  TEST_ASSERT_EQUAL('U', spi1_regfile.DATAR);
}

void test_spi_write_u16(void)
{
  pthread_t helper_thread;
  pthread_create(&helper_thread, NULL, test_spi_write_helper, NULL);
  spi_write_u16(SPI_Device1, 0xdead);
  pthread_join(helper_thread, NULL);

  TEST_ASSERT_EQUAL(0xdead, spi1_regfile.DATAR);
}

void *test_spi_read_helper(void *)
{
  sleep(1);
  spi1_regfile.DATAR = 0xdead;
  spi1_regfile.STATR |= U16_BIT(0);
}

void test_spi_read_u8(void)
{
  pthread_t helper_thread;
  pthread_create(&helper_thread, NULL, test_spi_write_helper, NULL);
  TEST_ASSERT_EQUAL(0xad, spi_read_u8(SPI_Device1));
  pthread_join(helper_thread, NULL);
}

void test_spi_read_u16(void)
{
  pthread_t helper_thread;
  pthread_create(&helper_thread, NULL, test_spi_write_helper, NULL);
  TEST_ASSERT_EQUAL(0xdead, spi_read_u16(SPI_Device1));
  pthread_join(helper_thread, NULL);
}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_spi_enable_disable_device_spi1_1line_tx_config);
  RUN_TEST(test_spi_configure_device);
  RUN_TEST(test_spi_reset_device);
  RUN_TEST(test_spi_write_u8);
  RUN_TEST(test_spi_write_u16);
  RUN_TEST(test_spi_read_u8);
  RUN_TEST(test_spi_read_u16);
  return UNITY_END();
}
