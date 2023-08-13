set(SRC_SOURCES
  time.c
  irq.c
  usb_device.c
  mmem.c
  msgq.c
  mmio_ops.c
  rcc_device.c
  gpio_device.c
  uart_device.c
  spi_device.c
  adf435x_control.c
  instances/drv/uart_instances.c
  instances/drv/spi_instances.c
)

add_library(src STATIC ${SRC_SOURCES})

target_include_directories(src PUBLIC
  "${PROJECT_SOURCE_DIR}/inc"
)
