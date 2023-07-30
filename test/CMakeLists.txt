add_executable(suite_mmio_ops
    test_mmio_ops.c
)

target_link_libraries(test_mmio_ops
  src
  Unity
)

add_test(suite_mmio_ops_test suite_mmio_ops )
