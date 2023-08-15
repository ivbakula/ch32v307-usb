#include <string.h>
#include <unity.h>

#include "Mockgpio_interface.h"
#include "Mockrcc_interface.h"
#include "Mockspi_interface.h"
#include "Mocktime.h"
#include "adf435x_device.h"

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

static const ADF435x_Regs regs = {
  .adf435x_regs = {0xdeadbeef, 0xacabface, 0xc0ffeeff, 0xfaceb004, 0xf00ba000, 0xaefbdfff},
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
  memset(&ADF435x_Instances[0], 0, sizeof(ADF435x_Instances));
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

  gpio_unlock_pin_ExpectAndReturn(uuid, PA1, GPIO_Success);
  gpio_unlock_pin_ExpectAndReturn(uuid, PA0, GPIO_Success);
  gpio_unlock_pin_ExpectAndReturn(uuid, PA2, GPIO_Success);

  TEST_ASSERT_EQUAL(ADF435x_Err_ConfigFail, adf435x_bind_device(ADF435x_Device1, SPI_Device1, ADF435x_PinConfig_Default));
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
  gpio_lock_pin_ExpectAndReturn(uuid, PA2, GPIO_Success);

  gpio_pin_config_Expect(PA1, GPIO_Mode_Output_50MHz, GPIO_Output_PushPull);
  gpio_pin_config_Expect(PA0, GPIO_Mode_Output_50MHz, GPIO_Output_PushPull);
  gpio_pin_config_Expect(PA2, GPIO_Mode_Input, GPIO_Input_PullUpDown);

  spi_packet_size_ExpectAndReturn(SPI_Device1, SPI_PACKET_TRANSFER_SIZE_8);

  TEST_ASSERT_EQUAL(ADF435x_Err_Success, adf435x_bind_device(ADF435x_Device1, SPI_Device1, ADF435x_PinConfig_Default));
  TEST_ASSERT_TRUE(ADF435x_Instances[0].is_binded);
  TEST_ASSERT_EQUAL(SPI_PACKET_TRANSFER_SIZE_8, ADF435x_Instances[0].spi_transfer_size);
  TEST_ASSERT_EQUAL(SPI_Device1, ADF435x_Instances[0].spi_bus);
}

void test_adf435x_enable_device_no_such_device_fail(void)
{
  adf435x_noof_instances = 0;
  TEST_ASSERT_EQUAL(ADF435x_Err_NoSuchDevice, adf435x_enable_device(ADF435x_Device1));
}

void test_adf435x_enable_device_not_binded_fail(void)
{
  TEST_ASSERT_EQUAL(ADF435x_Err_NotBinded, adf435x_enable_device(ADF435x_Device1));
}

void test_adf435x_enable_device_success(void)
{
  ADF435x_Instances[0].is_binded = true;
  gpio_pin_output_Expect(PA0, GPIO_HIGH);
  gpio_pin_output_Expect(PA1, GPIO_HIGH);
  TEST_ASSERT_EQUAL(ADF435x_Err_Success, adf435x_enable_device(ADF435x_Device1));
  TEST_ASSERT_TRUE(ADF435x_Instances[0].is_enabled);
}

void test_adf435x_disable_device_no_such_device_not_enabled_not_binded_fail(void)
{
  adf435x_noof_instances = 0;
  TEST_ASSERT_EQUAL(ADF435x_Err_NoSuchDevice, adf435x_disable_device(ADF435x_Device1));

  adf435x_noof_instances = 1;
  TEST_ASSERT_EQUAL(ADF435x_Err_NotBinded, adf435x_disable_device(ADF435x_Device1));

  ADF435x_Instances[0].is_binded = true;
  TEST_ASSERT_EQUAL(ADF435x_Err_NotEnabled, adf435x_disable_device(ADF435x_Device1));
}

void test_adf435x_disable_device_success(void)
{
  ADF435x_Instances[0].is_binded = true;
  ADF435x_Instances[0].is_enabled = true;

  gpio_pin_output_Expect(PA0, GPIO_LOW);
  gpio_pin_output_Expect(PA1, GPIO_LOW);

  TEST_ASSERT_EQUAL(ADF435x_Err_Success, adf435x_disable_device(ADF435x_Device1));
}

void test_adf435x_program_device_fail(void)
{
  adf435x_noof_instances = 0;
  TEST_ASSERT_EQUAL(ADF435x_Err_NoSuchDevice, adf435x_program_device(SPI_Device1, regs));

  adf435x_noof_instances = 1;
  adf435x_instantiated[0] = false;
  TEST_ASSERT_EQUAL(ADF435x_Err_NoSuchDevice, adf435x_program_device(SPI_Device1, regs));

  adf435x_instantiated[0] = true;
  TEST_ASSERT_EQUAL(ADF435x_Err_NotBinded, adf435x_program_device(SPI_Device1, regs));

  ADF435x_Instances[0].is_binded = true;
  TEST_ASSERT_EQUAL(ADF435x_Err_NotEnabled, adf435x_program_device(SPI_Device1, regs));
}

void test_adf435x_program_device_u8_success(void)
{
  ADF435x_Instances[0].is_binded = true;
  ADF435x_Instances[0].is_enabled = true;
  ADF435x_Instances[0].spi_bus = SPI_Device1;
  ADF435x_Instances[0].spi_transfer_size = SPI_PACKET_TRANSFER_SIZE_8;

  // register 5
  gpio_pin_output_Expect(PA1, GPIO_LOW);
  spi_write_Expect(SPI_Device1, 0xae);  // msb
  spi_write_Expect(SPI_Device1, 0xfb);
  spi_write_Expect(SPI_Device1, 0xdf);
  spi_write_Expect(SPI_Device1, 0xff);  // lsb
  wait_us_Expect(5);
  gpio_pin_output_Expect(PA1, GPIO_HIGH);

  // register 4
  gpio_pin_output_Expect(PA1, GPIO_LOW);
  spi_write_Expect(SPI_Device1, 0xf0);  // msb
  spi_write_Expect(SPI_Device1, 0x0b);
  spi_write_Expect(SPI_Device1, 0xa0);
  spi_write_Expect(SPI_Device1, 0x00);  // lsb
  wait_us_Expect(5);
  gpio_pin_output_Expect(PA1, GPIO_HIGH);

  // register 3
  gpio_pin_output_Expect(PA1, GPIO_LOW);
  spi_write_Expect(SPI_Device1, 0xfa);  // msb
  spi_write_Expect(SPI_Device1, 0xce);
  spi_write_Expect(SPI_Device1, 0xb0);
  spi_write_Expect(SPI_Device1, 0x04);  // lsb
  wait_us_Expect(5);
  gpio_pin_output_Expect(PA1, GPIO_HIGH);

  // register 2
  gpio_pin_output_Expect(PA1, GPIO_LOW);
  spi_write_Expect(SPI_Device1, 0xc0);  // msb
  spi_write_Expect(SPI_Device1, 0xff);
  spi_write_Expect(SPI_Device1, 0xee);
  spi_write_Expect(SPI_Device1, 0xff);  // lsb
  wait_us_Expect(5);
  gpio_pin_output_Expect(PA1, GPIO_HIGH);

  // register 1
  gpio_pin_output_Expect(PA1, GPIO_LOW);
  spi_write_Expect(SPI_Device1, 0xac);  // msb
  spi_write_Expect(SPI_Device1, 0xab);
  spi_write_Expect(SPI_Device1, 0xfa);
  spi_write_Expect(SPI_Device1, 0xce);  // lsb
  wait_us_Expect(5);
  gpio_pin_output_Expect(PA1, GPIO_HIGH);

  // register 0
  gpio_pin_output_Expect(PA1, GPIO_LOW);
  spi_write_Expect(SPI_Device1, 0xde);  // msb
  spi_write_Expect(SPI_Device1, 0xad);
  spi_write_Expect(SPI_Device1, 0xbe);
  spi_write_Expect(SPI_Device1, 0xef);  // lsb
  wait_us_Expect(5);
  gpio_pin_output_Expect(PA1, GPIO_HIGH);

  gpio_pin_input_ExpectAndReturn(PA2, GPIO_LOW);
  gpio_pin_input_ExpectAndReturn(PA2, GPIO_HIGH);

  TEST_ASSERT_EQUAL(ADF435x_Err_Success, adf435x_program_device(SPI_Device1, regs));
}

void test_adf435x_program_device_u16_success(void)
{
  ADF435x_Instances[0].is_binded = true;
  ADF435x_Instances[0].is_enabled = true;
  ADF435x_Instances[0].spi_bus = SPI_Device1;
  ADF435x_Instances[0].spi_transfer_size = SPI_PACKET_TRANSFER_SIZE_16;

  gpio_pin_output_Expect(PA1, GPIO_LOW);
  spi_write_Expect(SPI_Device1, 0xaefb);  // msb
  spi_write_Expect(SPI_Device1, 0xdfff);  // lsb
  wait_us_Expect(5);
  gpio_pin_output_Expect(PA1, GPIO_HIGH);

  // register 4
  gpio_pin_output_Expect(PA1, GPIO_LOW);
  spi_write_Expect(SPI_Device1, 0xf00b);  // msb
  spi_write_Expect(SPI_Device1, 0xa000);  // lsb
  wait_us_Expect(5);
  gpio_pin_output_Expect(PA1, GPIO_HIGH);

  // register 3
  gpio_pin_output_Expect(PA1, GPIO_LOW);
  spi_write_Expect(SPI_Device1, 0xface);  // msb
  spi_write_Expect(SPI_Device1, 0xb004);  // lsb
  wait_us_Expect(5);
  gpio_pin_output_Expect(PA1, GPIO_HIGH);

  // register 2
  gpio_pin_output_Expect(PA1, GPIO_LOW);
  spi_write_Expect(SPI_Device1, 0xc0ff);  // msb
  spi_write_Expect(SPI_Device1, 0xeeff);  // lsb
  wait_us_Expect(5);
  gpio_pin_output_Expect(PA1, GPIO_HIGH);

  // register 1
  gpio_pin_output_Expect(PA1, GPIO_LOW);
  spi_write_Expect(SPI_Device1, 0xacab);  // msb
  spi_write_Expect(SPI_Device1, 0xface);  // lsb
  wait_us_Expect(5);
  gpio_pin_output_Expect(PA1, GPIO_HIGH);

  // register 0
  gpio_pin_output_Expect(PA1, GPIO_LOW);
  spi_write_Expect(SPI_Device1, 0xdead);  // msb
  spi_write_Expect(SPI_Device1, 0xbeef);  // lsb
  wait_us_Expect(5);
  gpio_pin_output_Expect(PA1, GPIO_HIGH);

  gpio_pin_input_ExpectAndReturn(PA2, GPIO_LOW);
  gpio_pin_input_ExpectAndReturn(PA2, GPIO_HIGH);

  TEST_ASSERT_EQUAL(ADF435x_Err_Success, adf435x_program_device(SPI_Device1, regs));
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

  RUN_TEST(test_adf435x_disable_device_no_such_device_not_enabled_not_binded_fail);
  RUN_TEST(test_adf435x_disable_device_success);

  RUN_TEST(test_adf435x_program_device_fail);
  RUN_TEST(test_adf435x_program_device_u8_success);
  RUN_TEST(test_adf435x_program_device_u16_success);
  return UNITY_END();
}
