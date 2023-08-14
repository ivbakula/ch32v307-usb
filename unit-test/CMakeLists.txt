include(FetchContent)

FetchContent_Declare(
  Unity
  GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
  GIT_TAG v2.5.2
)

FetchContent_Declare(
  CMock
  GIT_REPOSITORY https://github.com/ThrowTheSwitch/CMock.git
  GIT_TAG v2.5.3
)

FetchContent_MakeAvailable(Unity CMock)

set(CMOCK_SOURCE_DIRECTORY ${cmock_SOURCE_DIR})
set(MOCK_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/mocks/src)
set(MOCK_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/mocks/inc)

add_library(Unity STATIC ${unity_SOURCE_DIR}/src/unity.c)
target_include_directories(Unity PUBLIC ${unity_SOURCE_DIR}/src)

add_library(CMock STATIC ${cmock_SOURCE_DIR}/src/cmock.c)
target_include_directories(CMock PUBLIC ${cmock_SOURCE_DIR}/src ${unity_SOURCE_DIR}/src)

add_subdirectory(test_mmio)
add_subdirectory(test_rcc)
add_subdirectory(test_gpio)
add_subdirectory(test_uart)
add_subdirectory(test_spi)
