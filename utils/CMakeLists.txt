set(UTIL_SOURCES
  time.c
  mmem.c
  msgq.c
  mmio.c
)

add_library(utils STATIC ${UTIL_SOURCES})

target_include_directories(utils PUBLIC
  "${PROJECT_SOURCE_DIR}/inc"
)
