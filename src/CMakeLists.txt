set(SRC_SOURCES
  time.c
  irq.c
  usb_device.c
  mmem.c
  msgq.c
  mmio_ops.c
  rcc_device.c
  gpio_device.c
)

add_library(src STATIC ${SRC_SOURCES})

target_include_directories(src PUBLIC
  "${PROJECT_SOURCE_DIR}/inc"
)
