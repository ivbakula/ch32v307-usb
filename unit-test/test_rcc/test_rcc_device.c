#include <unity.h>
#include "helpers.h"
#include "Mockmmio_ops.h"
#include "rcc_device.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_rcc_enable_disable_ahbpcenr(void)
{
  mmio_or_writedw_Expect(_RCC_REGISTER(R32_RCC_AHBPCENR), U32_BIT(17));
  rcc_pcendis(RCC_BLESEN, RCC_ENABLE);

  mmio_and_writedw_Expect(_RCC_REGISTER(R32_RCC_AHBPCENR), ~U32_BIT(17));
  rcc_pcendis(RCC_BLESEN, RCC_DISABLE);
}

void test_rcc_enable_disable_apb2pcenr(void)
{
  mmio_or_writedw_Expect(_RCC_REGISTER(R32_RCC_APB2PCENR), U32_BIT(12));
  rcc_pcendis(RCC_SPI1EN, RCC_ENABLE);

  mmio_and_writedw_Expect(_RCC_REGISTER(R32_RCC_APB2PCENR), ~U32_BIT(12));
  rcc_pcendis(RCC_SPI1EN, RCC_DISABLE);
}

void test_rcc_enable_disable_apb1pcenr(void)
{
  mmio_or_writedw_Expect(_RCC_REGISTER(R32_RCC_APB1PCENR), U32_BIT(17));
  rcc_pcendis(RCC_USART2EN, RCC_ENABLE);

  mmio_and_writedw_Expect(_RCC_REGISTER(R32_RCC_APB1PCENR), ~U32_BIT(17));
  rcc_pcendis(RCC_USART2EN, RCC_DISABLE);
}  

int main()
{
  UNITY_BEGIN();
  RUN_TEST(test_rcc_enable_disable_ahbpcenr);
  RUN_TEST(test_rcc_enable_disable_apb2pcenr);
  RUN_TEST(test_rcc_enable_disable_apb1pcenr);
  return UNITY_END();
}  
