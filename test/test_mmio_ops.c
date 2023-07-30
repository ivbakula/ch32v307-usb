#include <unity.h>
#include "mmio_ops.h"
#include "helpers.h"

#define UINTPTR_T(x) ((uintptr_t)&x)

void setUp(void)
{
}

void tearDown(void)
{
}

void test_mmio_readb(void)
{
  const uint32_t mmio_reg = 0xdeadbeef;
  TEST_ASSERT_EQUAL_HEX8(0xef, mmio_readb(UINTPTR_T(mmio_reg)));
  TEST_ASSERT_EQUAL_HEX8(0xbe, mmio_readb(UINTPTR_T(mmio_reg) + 1));
  TEST_ASSERT_EQUAL_HEX8(0xad, mmio_readb(UINTPTR_T(mmio_reg) + 2));
  TEST_ASSERT_EQUAL_HEX8(0xde, mmio_readb(UINTPTR_T(mmio_reg) + 3));
}

void test_mmio_readw(void)
{
  const uint32_t mmio_reg = 0xdeadbeef;
  TEST_ASSERT_EQUAL_HEX16(0xbeef, mmio_readw(UINTPTR_T(mmio_reg)));
  TEST_ASSERT_EQUAL_HEX16(0xadbe, mmio_readw(UINTPTR_T(mmio_reg) + 1));
  TEST_ASSERT_EQUAL_HEX16(0xdead, mmio_readw(UINTPTR_T(mmio_reg) + 2));
}

void test_mmio_readdw(void)
{
  const uint32_t mmio_reg = 0xdeadbeef;
  TEST_ASSERT_EQUAL_HEX32(0xdeadbeef, mmio_readdw(UINTPTR_T(mmio_reg)));
}

void test_mmio_writeb(void)
{
  const uint32_t mmio_reg = 0xdeadbeef;
  mmio_writeb(UINTPTR_T(mmio_reg), 0xff);
  TEST_ASSERT_EQUAL_HEX32(0xdeadbeff, mmio_reg);

  mmio_writeb(UINTPTR_T(mmio_reg) + 1, 0xff);
  TEST_ASSERT_EQUAL_HEX32(0xdeadffff, mmio_reg);

  mmio_writeb(UINTPTR_T(mmio_reg) + 2, 0xff);
  TEST_ASSERT_EQUAL_HEX32(0xdeffffff, mmio_reg);

  mmio_writeb(UINTPTR_T(mmio_reg) + 3, 0xff);
  TEST_ASSERT_EQUAL_HEX32(0xffffffff, mmio_reg);
}

void test_mmio_writew(void)
{
  const uint32_t mmio_reg = 0xdeadbeef;
  mmio_writew(UINTPTR_T(mmio_reg), 0xffff);
  TEST_ASSERT_EQUAL_HEX32(mmio_reg, 0xdeadffff);

  mmio_writew(UINTPTR_T(mmio_reg) + 1, 0x0000);
  TEST_ASSERT_EQUAL_HEX32(mmio_reg, 0xde0000ff);

  mmio_writew(UINTPTR_T(mmio_reg) + 2, 0xffff);
  TEST_ASSERT_EQUAL_HEX32(mmio_reg, 0xffff00ff);
}

void test_mmio_writedw(void)
{
  const uint32_t mmio_reg = 0xdeadbeef;
  mmio_writedw(UINTPTR_T(mmio_reg), 0xffffffff);
  TEST_ASSERT_EQUAL_HEX32(mmio_reg, 0xffffffff);
}

void test_mmio_or_writeb(void)
{
  const uint8_t mmio_reg = 0x00;
  mmio_or_writeb(UINTPTR_T(mmio_reg), U8_BIT(5));
  TEST_ASSERT_EQUAL(32, mmio_reg);
}

void test_mmio_or_writew(void)
{
  const uint32_t mmio_reg = 0x0000;
  mmio_or_writew(UINTPTR_T(mmio_reg), U16_BIT(10));
  TEST_ASSERT_EQUAL(1024, mmio_reg);
}

void test_mmio_or_writedw(void)
{
  const uint32_t mmio_reg = 0x00000000;
  mmio_or_writedw(UINTPTR_T(mmio_reg), U32_BIT(17));
  TEST_ASSERT_EQUAL_HEX32(0x20000, mmio_reg);
}

void test_mmio_and_writeb(void)
{
  const uint8_t mmio_reg = 0xff;
  mmio_and_writeb(UINTPTR_T(mmio_reg), ~U8_BIT(0));
  TEST_ASSERT_EQUAL(0xfe, mmio_reg);
}

void test_mmio_and_writew(void)
{
  const uint16_t mmio_reg = 0xffff;
  mmio_and_writew(UINTPTR_T(mmio_reg), ~U16_BIT(10));
  TEST_ASSERT_EQUAL_HEX16(0xfbff, mmio_reg);
}

void test_mmio_and_writedw(void)
{
  const uint32_t mmio_reg = 0xffffffff;
  mmio_and_writedw(UINTPTR_T(mmio_reg), ~U8_BIT(1));
  TEST_ASSERT_EQUAL_HEX32(0xfffffffd, mmio_reg);
}

int main(void)
{
  UNITY_BEGIN();

  RUN_TEST(test_mmio_readb);
  RUN_TEST(test_mmio_readw);
  RUN_TEST(test_mmio_readdw);

  RUN_TEST(test_mmio_writeb);
  RUN_TEST(test_mmio_writew);
  RUN_TEST(test_mmio_writedw);

  RUN_TEST(test_mmio_or_writeb);
  RUN_TEST(test_mmio_or_writew);
  RUN_TEST(test_mmio_or_writedw);

  RUN_TEST(test_mmio_and_writeb);
  RUN_TEST(test_mmio_and_writew);
  RUN_TEST(test_mmio_and_writedw);

  return UNITY_END();
}
