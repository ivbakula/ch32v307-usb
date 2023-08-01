#include <unity.h>
#include "helpers.h"

#include "rcc_device.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_rcc_register_macro(void)
{
  TEST_ASSERT_EQUAL(_RCC_REGISTER(R32_RCC_CTRL), &rcc_regfile.R32_RCC_CTRL);
  TEST_ASSERT_EQUAL(_RCC_REGISTER(R32_RCC_CFGR0), &rcc_regfile.R32_RCC_CFGR0);
  TEST_ASSERT_EQUAL(_RCC_REGISTER(R32_RCC_INTR), &rcc_regfile.R32_RCC_INTR);
  TEST_ASSERT_EQUAL(_RCC_REGISTER(R32_RCC_APB2PRSTR), &rcc_regfile.R32_RCC_APB2PRSTR);
  TEST_ASSERT_EQUAL(_RCC_REGISTER(R32_RCC_APB1PRSTR), &rcc_regfile.R32_RCC_APB1PRSTR);
  TEST_ASSERT_EQUAL(_RCC_REGISTER(R32_RCC_AHBPCENR), &rcc_regfile.R32_RCC_AHBPCENR);
  TEST_ASSERT_EQUAL(_RCC_REGISTER(R32_RCC_APB2PCENR), &rcc_regfile.R32_RCC_APB2PCENR);
  TEST_ASSERT_EQUAL(_RCC_REGISTER(R32_RCC_APB1PCENR), &rcc_regfile.R32_RCC_APB1PCENR);
  TEST_ASSERT_EQUAL(_RCC_REGISTER(R32_RCC_BDCTRL), &rcc_regfile.R32_RCC_BDCTRL);
  TEST_ASSERT_EQUAL(_RCC_REGISTER(R32_RCC_RSTSCKR), &rcc_regfile.R32_RCC_RSTSCKR);
  TEST_ASSERT_EQUAL(_RCC_REGISTER(R32_RCC_AHBRSTR), &rcc_regfile.R32_RCC_AHBRSTR);
  TEST_ASSERT_EQUAL(_RCC_REGISTER(R32_RCC_CFGR2), &rcc_regfile.R32_RCC_CFGR2);
}

void test_rcc_enable_disable_single_ahbpcenr_periph(void)
{
  rcc_pcendis(RCC_BLESEN, RCC_ENABLE);
  TEST_ASSERT_EQUAL(rcc_regfile.R32_RCC_AHBPCENR, U32_BIT(17));

  rcc_pcendis(RCC_BLESEN, RCC_DISABLE);
  TEST_ASSERT_EQUAL(rcc_regfile.R32_RCC_AHBPCENR, 0);

  rcc_pcendis(RCC_ETHMACRXEN, RCC_ENABLE);
  TEST_ASSERT_EQUAL(rcc_regfile.R32_RCC_AHBPCENR, U32_BIT(16));

  rcc_pcendis(RCC_ETHMACRXEN, RCC_DISABLE);
  TEST_ASSERT_EQUAL(rcc_regfile.R32_RCC_AHBPCENR, 0);
}

void test_rcc_enable_disable_multiple_ahbpcenr_periph(void)
{
  rcc_pcendis(RCC_ETHMACTXEN, RCC_ENABLE);
  rcc_pcendis(RCC_ETHMACEN, RCC_ENABLE);
  TEST_ASSERT_EQUAL(rcc_regfile.R32_RCC_AHBPCENR, U32_BIT(14) | U32_BIT(15));

  rcc_pcendis(RCC_ETHMACTXEN, RCC_DISABLE);
  TEST_ASSERT_EQUAL(rcc_regfile.R32_RCC_AHBPCENR, U32_BIT(14));

  rcc_pcendis(RCC_ETHMACEN, RCC_DISABLE);
  TEST_ASSERT_EQUAL(rcc_regfile.R32_RCC_AHBPCENR, 0);
}

void test_rcc_enable_disable_apb2pcenr_periph(void)
{
  rcc_pcendis(RCC_SPI1EN, RCC_ENABLE);
  TEST_ASSERT_EQUAL(rcc_regfile.R32_RCC_APB2PCENR, U32_BIT(12));

  rcc_pcendis(RCC_SPI1EN, RCC_DISABLE);
  TEST_ASSERT_EQUAL(rcc_regfile.R32_RCC_APB2PCENR, 0);
}

void test_rcc_enable_disable_apb1pcenr_periph(void)
{
  rcc_pcendis(RCC_USART2EN, RCC_ENABLE);
  TEST_ASSERT_EQUAL(rcc_regfile.R32_RCC_APB1PCENR, U32_BIT(17));

  rcc_pcendis(RCC_USART2EN, RCC_DISABLE);
  TEST_ASSERT_EQUAL(rcc_regfile.R32_RCC_APB1PCENR, 0);
}

int main(void)
{
  UNITY_BEGIN();

  RUN_TEST(test_rcc_register_macro);
  RUN_TEST(test_rcc_enable_disable_single_ahbpcenr_periph);
  RUN_TEST(test_rcc_enable_disable_multiple_ahbpcenr_periph);
  RUN_TEST(test_rcc_enable_disable_apb2pcenr_periph);
  RUN_TEST(test_rcc_enable_disable_apb1pcenr_periph);
  
  return UNITY_END();
}  
