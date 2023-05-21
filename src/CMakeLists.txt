set(SRC_SOURCES
  time.c
  irq.c
  usb.c
  mmem.c
  msgq.c
)

add_library(src STATIC ${SRC_SOURCES})

target_include_directories(src PUBLIC
  "${PROJECT_SOURCE_DIR}/inc"
)
