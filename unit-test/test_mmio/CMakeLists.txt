add_executable(test_mmio_suite
    test_mmio_suite.c
    "${PROJECT_SOURCE_DIR}/src/mmio_ops.c"
)

target_link_libraries(test_mmio_suite
  Unity
)

target_compile_options(test_mmio_suite PUBLIC
   -ggdb -D UNITTEST
)

target_include_directories(test_mmio_suite PUBLIC
  "${PROJECT_SOURCE_DIR}/inc"
)

add_test(test_mmio_suite test_mmio_suite)
