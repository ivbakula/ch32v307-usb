#include "uart_device.h"
#include "uart_interface.h"
#include "rcc_interface.h"
#include "mmio.h"

static uint16_t calculate_brr(uint32_t apbclk_freq, uint32_t baud)
{
  uint16_t over8div = 4;
  uint16_t integer_divider = (25 * (apbclk_freq)) / ((over8div) * (baud));
  uint16_t fractional_divider = integer_divider % 100;

  return ((((integer_divider) / 100) << 4) | (((((fractional_divider) * ((over8div)*2)) + 50)/100)&7));
}

static inline uint32_t get_rcc_mapping(USART_Iface ) __attribute__((always_inline));
static inline uint32_t get_rcc_mapping(USART_Iface iface)
{
  if (iface < USART_MAX)
    return __usart_mmio_mapping[iface];

  return -1;
  // do something about this obvious error. Throw an exception or put red light on or something like that
}

static inline uint32_t get_mmio_mapping(USART_Iface iface) __attribute__((always_inline));
static inline uint32_t get_mmio_mapping(USART_Iface iface)
{
  
}

USART_Instance *usart_create_instance(USART_Iface iface, uint32_t apbclk_freq, uint16_t baud)
{
  uint32_t uart_rcc_dev, uart_mmio_base;

  uart_rcc_dev = get_rcc_mapping(iface);
  if (uart_rcc_dev == -1) {
    // do something about this
    return 0;
  }
  
  uart_mmio_base = get_mmio_mapping(iface);
  if (uart_mmio_base == -1) {
    // do something about this
    return 0;
  }
  
  rcc_enable_periph(uart_rcc_dev);

  // TODO do stuff with GPIO port. This step depends on GPIO driver, which is on TODO list
  // gpio_claim_device()

  mmio_write_u32(uart_mmio_base + USART_CTRL1_OFFSET, U32_BIT(3) | U32_BIT(2));
  mmio_write_u32(uart_mmio_base + USART_CTRL2_OFFSET, 0);
  mmio_write_u32(uart_mmio_base + USART_CTRL3_OFFSET, 0);
  mmio_write_u32(uart_mmio_base + USART_BRR_OFFSET, calculate_brr(apbclk_freq, baud));
}

void usart_putc(USART_Iface iface, char c)
{
  
}

char usart_getc(USART_Iface iface)
{
  
}  
