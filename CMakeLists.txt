cmake_minimum_required(VERSION 3.15)

set(PROJECT firmware)
project(${PROJECT} LANGUAGES C ASM)

if (NOT TOOLCHAIN_PREFIX)
  message(INFO " TOOLCHAIN_PREFIX is not set. Using default \"riscv-none-embed-\"")
  set(TOOLCHAIN_PREFIX riscv-none-embed-)
endif ()

if (TARGET_GROUP STREQUAL test)
   message(INFO "Build unit tests")
   include(CTest)
   add_subdirectory(external)
   add_subdirectory(test)
else ()
    message(INFO "This is not a test :D. Build the real thing.")
    set(CMAKE_C_COMPILER "${TOOLCHAIN_PREFIX}gcc")

    set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")


    set(PROJECT_TARGET_ELF "${PROJECT}.elf")

    set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)

    add_subdirectory(boot)
    add_subdirectory(src)

    set(MCU_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/ch32v307.ld)

    set(RISCV_CH32V30x_CPU
      -march=rv32imafc
      -mabi=ilp32
      -msmall-data-limit=8
      -msave-restore
      -fmessage-length=0
      -fsigned-char
      -ffunction-sections
      -fdata-sections
      -fno-common
    )

    set(CMAKE_C_FLAGS -Os -Wunused -Wuninitialized -g)

    target_compile_options(${PROJECT_TARGET_ELF} PUBLIC
      ${RISCV_CH32V30x_CPU}
    )

    target_link_options(${PROJECT_TARGET_ELF} PUBLIC
      -T${MCU_LINKER_SCRIPT}
      -nostartfiles
      -Xlinker
      --gc-sections
      -Wl,-Map=${PROJECT}.map
      --specs=nano.specs
      --specs=nosys.specs
    )

    add_custom_target(flashit COMMAND wchisp flash ${PROJECT_BINARY_DIR}/${PROJECT}.hex COMMENT "flashes it")
endif ()
