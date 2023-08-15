#include <unity.h>
#include "adf435x_device.h"
#include "Mockrcc_interface.h"
#include "Mockgpio_interface.h"
#include "Mockspi_interface.h"

// clang-format off
size_t adf435x_noof_instances = 1;
bool adf435x_instantiated[1] = { 1 };
ADF435x_Instance ADF435x_Instances[1] = {
  {
    .uuid = 0xdeadbeef,
    .spi_bus = 0,
    .is_binded = false,
    .is_enabled = false,
    .spi_transfer_size = 0,
    .no_pin_config = ADF435x_MAX_PINCONFIG,
    .chosen_pinconfig = ADF435x_PinConfig_Default,
    .pin_configuration = {
     /* LE   CE   LD  */
      { PA1, PA0, PA2 },
    },
  },
};
// clang-format on

static const GPIO_Pin pin_configuration[1][3] = {
  /* LE   CE   LD  */
  {PA1, PA0, PA2},
};

void setUp(void)
{
  ADF435x_Instances[0].uuid = 0xdeadbeef;
  ADF435x_Instances[0].spi_bus = 0;
  ADF435x_Instances[0].is_binded = false;
  ADF435x_Instances[0].is_enabled = false;
  ADF435x_Instances[0].spi_transfer_size = 0;
  ADF435x_Instances[0].no_pin_config = ADF435x_MAX_PINCONFIG;
  ADF435x_Instances[0].chosen_pinconfig = ADF435x_PinConfig_Default;
  memcpy(&ADF435x_Instances[0].pin_configuration[0][0], &pin_configuration[0][0], sizeof(pin_configuration));

  adf435x_noof_instances = 1;
  adf435x_instantiated[0] = true;
}

void tearDown(void)
{
  memset(&ADF435x_Instances[0].pin_configuration[0][0], 0, sizeof(ADF435x_Instances));
}

void test_adf435x_bind_device_no_such_device_fail(void)
{
  adf435x_noof_instances = 0;
  adf435x_instantiated[0] = false;
  TEST_ASSERT_EQUAL(ADF435x_Err_NoSuchDevice, adf435x_bind_device(ADF435x_Device1, SPI_Device1, ADF435x_PinConfig_Default));
  
  adf435x_noof_instances = 1;
  TEST_ASSERT_EQUAL(ADF435x_Err_NoSuchDevice, adf435x_bind_device(ADF435x_Device1, SPI_Device1, ADF435x_PinConfig_Default));
}

void test_adf435x_bind_device_spi_not_enabled_fail(void)
{
  spi_is_enabled_ExpectAndReturn(SPI_Device1, false);
  TEST_ASSERT_EQUAL(ADF435x_Err_ConfigFail, adf435x_bind_device(ADF435x_Device1, SPI_Device1, ADF435x_PinConfig_Default));  
}

void test_adf435x_bind_device_spi_not_configured_fail(void)
{
  spi_is_enabled_ExpectAndReturn(SPI_Device1, true);
  spi_is_configured_ExpectAndReturn(SPI_Device1, false);

  TEST_ASSERT_EQUAL(ADF435x_Err_ConfigFail, adf435x_bind_device(ADF435x_Device1, SPI_Device1, ADF435x_PinConfig_Default));
}

void test_adf435x_bind_device_invalid_pinconfig_fail(void)
{
  ADF435x_Instances[0].no_pin_config = 0;
  spi_is_enabled_ExpectAndReturn(SPI_Device1, true);
  spi_is_configured_ExpectAndReturn(SPI_Device1, true);

  TEST_ASSERT_EQUAL(ADF435x_Err_ConfigFail, adf435x_bind_device(ADF435x_Device1, SPI_Device1, ADF435x_PinConfig_Default));
}

void test_adf435x_bind_device_pinlock_fail(void)
{
  const uint32_t uuid = ADF435x_Instances[0].uuid;
  spi_is_enabled_ExpectAndReturn(SPI_Device1, true);
  spi_is_configured_ExpectAndReturn(SPI_Device1, true);
  
  gpio_port_enable_Expect(GET_GPIO_PORT(PA1));
  gpio_lock_pin_ExpectAndReturn(uuid, PA1, GPIO_Success);
  gpio_port_enable_Expect(GET_GPIO_PORT(PA0));
  gpio_lock_pin_ExpectAndReturn(uuid, PA0, GPIO_Success);
  gpio_port_enable_Expect(GET_GPIO_PORT(PA2));
  gpio_lock_pin_ExpectAndReturn(uuid, PA2, GPIO_Locked);

  gpio_unlock_pin_ExpectAndReturn(PA1, GPIO_Sucess);
  gpio_unlock_pin_ExpectAndReturn(PA0, GPIO_Sucess);
  gpio_unlock_pin_ExpectAndReturn(PA2, GPIO_Sucess);

  TEST_ASSERT_EQUAL(ADF435x_Err_ConfigFail, adf435x_bind_device(ADF435x_Device, SPI_Device1, ADF435x_PinConfig_Default));  
}

void test_adf435x_bind_device_success(void)
{
  const uint32_t uuid = ADF435x_Instances[0].uuid;
  spi_is_enabled_ExpectAndReturn(SPI_Device1, true);
  spi_is_configured_ExpectAndReturn(SPI_Device1, true);

  gpio_port_enable_Expect(GET_GPIO_PORT(PA1));
  gpio_lock_pin_ExpectAndReturn(uuid, PA1, GPIO_Success);
  gpio_port_enable_Expect(GET_GPIO_PORT(PA0));
  gpio_lock_pin_ExpectAndReturn(uuid, PA0, GPIO_Success);
  gpio_port_enable_Expect(GET_GPIO_PORT(PA2));
  gpio_lock_pin_ExpectAndReturn(uuid, PA0, GPIO_Success);

  gpio_pin_config_Expect(PA1, GPIO_Mode_Output_50MHz, GPIO_Output_PushPull);
  gpio_pin_config_Expect(PA0, GPIO_Mode_Input, GPIO_Input_PullUpDown);
  gpio_pin_config_Expect(PA2, GPIO_Mode_Output_50MHz, GPIO_Output_PushPull);
  spi_packet_size_ExpectAndReturn(SPI_Device1, SPI_PACKET_TRANSFER_SIZE_8);
  
  TEST_ASSERT_EQUAL(ADF435x_Err_Success, adf435x_bind_device(ADF435x_Device, SPI_Device1, ADF435x_PinConfig_Default));
  TEST_ASSERT_TRUE(ADF435x_Instances[0].is_binded);
  TEST_ASSERT_EQUAL(SPI_PACKET_TRANSFER_SIZE_8, ADF435x_Instances[0].spi_transfer_size);
  TEST_ASSERT_EQUAL(SPI_Device1, ADF435x_Instances[0].spi_bus);
}

void test_adf435x_enable_device_no_such_device_fail(void)
{
}

void test_adf435x_enable_device_not_binded_fail(void)
{
}

void test_adf435x_enable_device_success(void)
{
}

void test_adf435x_disable_device_no_such_device_fail(void)
{
}

void test_adf435x_disable_device_not_binded_fail(void)
{
}

void test_adf435x_disable_device_not_enabled_fail(void)
{
}

void test_adf435x_disable_device_success(void)
{
}

void test_adf435x_program_device_no_such_device_fail(void)
{
}

void test_adf435x_program_device_success(void)
{
}

int main()
{
  UNITY_BEGIN();
  RUN_TEST(test_adf435x_bind_device_no_such_device_fail);
  RUN_TEST(test_adf435x_bind_device_spi_not_enabled_fail);
  RUN_TEST(test_adf435x_bind_device_spi_not_configured_fail);
  RUN_TEST(test_adf435x_bind_device_invalid_pinconfig_fail);
  RUN_TEST(test_adf435x_bind_device_pinlock_fail);
  RUN_TEST(test_adf435x_bind_device_success);

  RUN_TEST(test_adf435x_enable_device_no_such_device_fail);
  RUN_TEST(test_adf435x_enable_device_not_binded_fail);
  RUN_TEST(test_adf435x_enable_device_success);

  RUN_TEST(test_adf435x_disable_device_no_such_device_fail);
  RUN_TEST(test_adf435x_disable_device_not_binded_fail);
  RUN_TEST(test_adf435x_disable_device_not_enabled_fail);
  RUN_TEST(test_adf435x_disable_device_success);

  RUN_TEST(test_adf435x_program_device_no_such_device_fail);
  RUN_TEST(test_adf435x_program_device_success);
  return UNITY_END();
}


