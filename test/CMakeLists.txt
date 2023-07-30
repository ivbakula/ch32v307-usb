add_executable(suite_mmio_ops
    test_mmio_ops.c
    "${PROJECT_SOURCE_DIR}/src/mmio_ops.c"
)

target_link_libraries(suite_mmio_ops
  Unity
)

target_include_directories(suite_mmio_ops PUBLIC
  "${PROJECT_SOURCE_DIR}/inc"
)

add_test(suite_mmio_ops_test suite_mmio_ops )
