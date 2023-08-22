#include <string.h>

#include "Mockgpio_interface.h"
#include "Mockirq.h"
#include "Mockmmio_ops.h"
#include "Mockrcc_interface.h"
#include "dvp_device.h"

bool dvp_instantiated[1] = {1};
size_t no_of_dvp_instances = 1;
DVP_Instance DVP_Instances[1] = {0};

static GPIO_Pin pin_configurations_dvp[DVP_MAX_PINCONFIGS][16] = {
  {PA4, PA5, PA6, PA9, PA10, PC8, PC9, PC11, PB6, PB8, PB9, PC10, PC12, PD6, PD2},
  {PA4, PA5, PA6, PA9, PA10, PC8, PC9, PC11, PB3, PB8, PB9, PC10, PC12, PD6, PD2},
};

void setUp(void)
{
  no_of_dvp_instances = 1;
  dvp_instantiated[0] = true;
  DVP_Instances[0].base = DVP_BASE;
  DVP_Instances[0].rcc_devid = RCC_DVPEN;
  DVP_Instances[0].irqn = DVP_IRQn;
  DVP_Instances[0].enabled = false;
  DVP_Instances[0].configured = false;
  DVP_Instances[0].chosen_pinconfig = DVP_DEFAULT_MAPPING;
  DVP_Instances[0].no_pin_configs = DVP_MAX_PINCONFIGS;
  memcpy(&DVP_Instances[0].pin_configuration, pin_configurations_dvp, sizeof(pin_configurations_dvp));
}

void tearDown(void)
{
  memset(&DVP_Instances[0], 0, sizeof(DVP_Instances));
}

void test_dvp_enable_failure(void)
{
  // Device out of range
  no_of_dvp_instances = 0;
  TEST_ASSERT_EQUAL(DVP_Err_DeviceOutOfRange, dvp_enable_device(DVP_Device1, DVP_DEFAULT_MAPPING));

  // No such device failure
  no_of_dvp_instances = 1;
  dvp_instantiated[0] = false;
  TEST_ASSERT_EQUAL(DVP_Err_NoSuchDevice, dvp_enable_device(DVP_Device1, DVP_DEFAULT_MAPPING));

  // Already enabled failure
  dvp_instantiated[0] = true;
  DVP_Instances[0].enabled = true;
  TEST_ASSERT_EQUAL(DVP_Err_AlreadyEnabled, dvp_enable_device(DVP_Device1, DVP_DEFAULT_MAPPING));

  // Invalid pinconfig failure
  DVP_Instances[0].enabled = false;
  dvp_instantiated[0] = true;
  DVP_Instances[0].no_pin_configs = 0;
  TEST_ASSERT_EQUAL(DVP_Err_ConfigFail, dvp_enable_device(DVP_Device1, DVP_DEFAULT_MAPPING));

  // Pin locked failure
  DVP_Instances[0].no_pin_configs = DVP_MAX_PINCONFIGS;
  rcc_pcendis_Expect(RCC_DVPEN, RCC_ENABLE);

  // Expect Enable port A
  gpio_port_enable_Expect(GET_GPIO_PORT(PA4));
  gpio_lock_pin_ExpectAndReturn(RCC_DVPEN, PA4, GPIO_Success);
  gpio_pin_config_Expect(PA4, GPIO_Mode_Input, GPIO_Input_PullUpDown);
  gpio_pin_pullstate_config_Expect(PA4, GPIO_PullState_Up);

  gpio_port_enable_Expect(GET_GPIO_PORT(PA5));
  gpio_lock_pin_ExpectAndReturn(RCC_DVPEN, PA5, GPIO_Ownership);

  // Expect PA4 pin unlock
  for (int i = 0; i < 16; i++) {
    gpio_unlock_pin_ExpectAndReturn(RCC_DVPEN, pin_configurations_dvp[DVP_DEFAULT_MAPPING][i], GPIO_Success);
  }

  TEST_ASSERT_EQUAL(DVP_Err_ConfigFail, dvp_enable_device(DVP_Device1, DVP_DEFAULT_MAPPING));
}

void test_dvp_enable_success(void)
{
  rcc_pcendis_Expect(RCC_DVPEN, RCC_ENABLE);

  for (int i = 0; i < 16; i++) {
    gpio_port_enable_Expect(GET_GPIO_PORT(pin_configurations_dvp[DVP_DEFAULT_MAPPING][i]));
    gpio_lock_pin_ExpectAndReturn(RCC_DVPEN, pin_configurations_dvp[DVP_DEFAULT_MAPPING][i], GPIO_Success);
    gpio_pin_config_Expect(pin_configurations_dvp[DVP_DEFAULT_MAPPING][i], GPIO_Mode_Input, GPIO_Input_PullUpDown);
    gpio_pin_pullstate_config_Expect(pin_configurations_dvp[DVP_DEFAULT_MAPPING][i], GPIO_PullState_Up);
  }

  TEST_ASSERT_EQUAL(DVP_Err_Success, dvp_enable_device(DVP_Device1, DVP_DEFAULT_MAPPING));
  TEST_ASSERT_TRUE(DVP_Instances[0].enabled);
}

void test_dvp_configure_failure(void)
{
  const DVP_Config dvp_config;
  // Device out of range
  no_of_dvp_instances = 0;
  TEST_ASSERT_EQUAL(DVP_Err_DeviceOutOfRange, dvp_configure_device(DVP_Device1, dvp_config));

  // No such device failure
  no_of_dvp_instances = 1;
  dvp_instantiated[0] = false;
  TEST_ASSERT_EQUAL(DVP_Err_NoSuchDevice, dvp_configure_device(DVP_Device1, dvp_config));

  // Not enabled failure
  dvp_instantiated[0] = true;
  DVP_Instances[0].enabled = false;
  TEST_ASSERT_EQUAL(DVP_Err_NotEnabled, dvp_configure_device(DVP_Device1, dvp_config));

  // Already configured failure
  DVP_Instances[0].enabled = true;
  DVP_Instances[0].configured = true;
  TEST_ASSERT_EQUAL(DVP_Err_AlreadyConfigured, dvp_configure_device(DVP_Device1, dvp_config));
}

void irq_handler_dvp(void);

void test_dvp_configure_success(void)
{
  DVP_Instances[0].enabled = true;
  uint16_t buff0[1024];
  uint16_t buff1[1024];

  const DVP_Config dvp_config = {
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
    .DVP_COL_NUM = 1024,
    .DVP_DMA_BUF0 = (uintptr_t)((uint16_t *)buff0),
    .DVP_DMA_BUF1 = (uintptr_t)((uint16_t *)buff1),
  };

  /* 1. clear bus width setting in control register 0 */
  mmio_and_writeb_Expect(_DVP_REGISTER(DVP_BASE, R8_DVP_CR0), ~0x30);

  /* 2. clear FIFO reset and receive logic reset bits in control register 1 */
  mmio_and_writeb_Expect(_DVP_REGISTER(DVP_BASE, R8_DVP_CR1), ~(U8_BIT(1) | U8_BIT(2)));

  /* 3. configure DVP receiver device; (bus width, PCLK, HSYNC and VSYNC polarity and data format)
   * Don't enable DVP device for now (the main reason for clearing bit 0)
   */
  mmio_writeb_Expect(_DVP_REGISTER(DVP_BASE, R8_DVP_CR0), U8_BIT(5));

  /* 4. configure DVP receiver device; capture mode, crop function, and frame capture rate.
   * Don't enable DMA for now (the main reason for clearing bit 0)
   */
  mmio_or_writeb_Expect(_DVP_REGISTER(DVP_BASE, R8_DVP_CR1), U8_BIT(4));

  /* 5. configure number of pixels (samples) per row. */
  mmio_writew_Expect(_DVP_REGISTER(DVP_BASE, R16_DVP_COL_NUM), 1024);

  /* 6. configure row numbers per frame */
  mmio_writew_Expect(_DVP_REGISTER(DVP_BASE, R16_DVP_ROW_NUM), 1);

  /* 7. configure DMA buffer0 */
  mmio_writedw_Expect(_DVP_REGISTER(DVP_BASE, R32_DVP_DMA_BUF0), ((uintptr_t)buff0) & 0xffffffff);

  /* 8. configure DMA buffer0 */
  mmio_writedw_Expect(_DVP_REGISTER(DVP_BASE, R32_DVP_DMA_BUF1), ((uintptr_t)buff1) & 0xffffffff);

  /* 9. enable selected interrupts */
  mmio_writeb_Expect(_DVP_REGISTER(DVP_BASE, R8_DVP_IER), 0x1f);

  irq_register_interrupt_handler_Expect(DVP_IRQn, irq_handler_dvp);
  irq_enable_interrupt_Expect(DVP_IRQn);

  mmio_or_writeb_Expect(_DVP_REGISTER(DVP_BASE, R8_DVP_CR1), U8_BIT(0));
  mmio_or_writeb_Expect(_DVP_REGISTER(DVP_BASE, R8_DVP_CR0), U8_BIT(0));

  TEST_ASSERT_EQUAL(DVP_Err_Success, dvp_configure_device(DVP_Device1, dvp_config));
  TEST_ASSERT_TRUE(DVP_Instances[0].configured);
}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_dvp_enable_failure);
  RUN_TEST(test_dvp_enable_success);
  RUN_TEST(test_dvp_configure_failure);
  RUN_TEST(test_dvp_configure_success);
  return UNITY_END();
}
