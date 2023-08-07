#include <unity.h>
#include <pthread.h>
#include <unistd.h>

#include "spi_device.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_spi_enable_disable_device(void)
{
}

void test_spi_configure_device(void)
{
}

void test_spi_reset_device(void)
{
}

void test_spi_write_u8(void)
{
}

void test_spi_write_u16(void)
{
}

void test_spi_read_u8(void)
{
}

void test_spi_read_u16(void)
{
}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_spi_enable_disable_device);
  RUN_TEST(test_spi_configure_device);
  RUN_TEST(test_spi_reset_device);
  RUN_TEST(test_spi_write_u8);
  RUN_TEST(test_spi_write_u16);
  RUN_TEST(test_spi_read_u8);
  RUN_TEST(test_spi_read_u16);
  return UNITY_END();
}  

