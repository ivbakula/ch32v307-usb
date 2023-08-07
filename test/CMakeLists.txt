##
## TEST_MMIO_OPS
##
add_executable(suite_mmio_ops
    test_mmio_ops.c
    "${PROJECT_SOURCE_DIR}/src/mmio_ops.c"
)

target_link_libraries(suite_mmio_ops
  Unity
)

target_compile_options(suite_mmio_ops PUBLIC
   -ggdb -D UNITTEST
)

target_include_directories(suite_mmio_ops PUBLIC
  "${PROJECT_SOURCE_DIR}/inc"
)

add_test(suite_mmio_ops_test suite_mmio_ops)


##
## TEST_RCC_DEVICE
##
add_executable(suite_rcc_device
    test_rcc_device.c
    "${PROJECT_SOURCE_DIR}/src/rcc_device.c"
    "${PROJECT_SOURCE_DIR}/src/mmio_ops.c"
)

target_link_libraries(suite_rcc_device
  Unity
)

target_compile_options(suite_rcc_device PUBLIC
  -ggdb -D UNITTEST
)

target_include_directories(suite_rcc_device PUBLIC
  "${PROJECT_SOURCE_DIR}/inc"
  "${PROJECT_SOURCE_DIR}/src"
)

add_test(suite_rcc_device_test suite_rcc_device)


##
## TEST_GPIO
##
add_executable(suite_gpio_device
    test_gpio_device.c
    "${PROJECT_SOURCE_DIR}/src/gpio_device.c"
    "${PROJECT_SOURCE_DIR}/src/rcc_device.c"
    "${PROJECT_SOURCE_DIR}/src/mmio_ops.c"
)

target_link_libraries(suite_gpio_device
  Unity
)

target_compile_options(suite_gpio_device PUBLIC
  -ggdb -D UNITTEST
)

target_include_directories(suite_gpio_device PUBLIC
  "${PROJECT_SOURCE_DIR}/inc"
  "${PROJECT_SOURCE_DIR}/src"
)

add_test(suite_gpio_device_test suite_gpio_device)

##
## TEST_UART
##
add_executable(suite_uart_device
    test_uart_device.c
    "${PROJECT_SOURCE_DIR}/src/uart_device.c"
    "${PROJECT_SOURCE_DIR}/src/instances/drv/uart_instances.c"
    "${PROJECT_SOURCE_DIR}/src/gpio_device.c"
    "${PROJECT_SOURCE_DIR}/src/rcc_device.c"
    "${PROJECT_SOURCE_DIR}/src/mmio_ops.c"
)

target_link_libraries(suite_uart_device
  Unity
)

target_compile_options(suite_uart_device PUBLIC
  -lpthread -ggdb -D UNITTEST
)

target_include_directories(suite_uart_device PUBLIC
  "${PROJECT_SOURCE_DIR}/inc"
  "${PROJECT_SOURCE_DIR}/src"
)

add_test(suite_uart_device_test suite_uart_device)


##
## TEST_SPI
##
add_executable(suite_spi_device
    test_spi_device.c
    "${PROJECT_SOURCE_DIR}/src/spi_device.c"
    "${PROJECT_SOURCE_DIR}/src/instances/drv/spi_instances.c"
    "${PROJECT_SOURCE_DIR}/src/gpio_device.c"
    "${PROJECT_SOURCE_DIR}/src/rcc_device.c"
    "${PROJECT_SOURCE_DIR}/src/mmio_ops.c"
)

target_link_libraries(suite_spi_device
  Unity
)

target_compile_options(suite_spi_device PUBLIC
  -lpthread -ggdb -D UNITTEST
)

target_include_directories(suite_spi_device PUBLIC
  "${PROJECT_SOURCE_DIR}/inc"
  "${PROJECT_SOURCE_DIR}/src"
)

add_test(suite_spi_device_test suite_spi_device)


