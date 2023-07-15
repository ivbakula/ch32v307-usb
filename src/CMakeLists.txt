set(SRC_SOURCES
  irq.c
  usb_device.c
)

add_library(src STATIC ${SRC_SOURCES})

target_include_directories(src PUBLIC
  "${PROJECT_SOURCE_DIR}/inc"
  "${PROJECT_SOURCE_DIR}/utils}"
)
