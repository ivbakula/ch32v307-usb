add_executable(test_uart_device_suite
  test_uart_device.c
  "${PROJECT_SOURCE_DIR}/src/uart_device.c"
  "${MOCK_SOURCE_DIR}/Mockmmio_ops.c"
  "${MOCK_SOURCE_DIR}/Mockrcc_interface.c"
  "${MOCK_SOURCE_DIR}/Mockgpio_interface.c"
  "${MOCK_SOURCE_DIR}/Mockirq.c"
)

target_link_libraries(test_uart_device_suite
  Unity
  CMock
)

target_compile_options(test_uart_device_suite PUBLIC
  -ggdb
)

target_include_directories(test_uart_device_suite PUBLIC
  "${PROJECT_SOURCE_DIR}/inc"
  "${PROJECT_SOURCE_DIR}/src"
  "${CMOCK_SOURCE_DIRECTORY}/src"
  "${MOCK_INCLUDE_DIR}"
)

add_test(test_uart_device_suite test_uart_device_suite)
