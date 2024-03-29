/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCKSPI_INTERFACE_H
#define _MOCKSPI_INTERFACE_H

#include "unity.h"
#include "spi_interface.h"

/* Ignore the following warnings, since we are copying code */
#if defined(__GNUC__) && !defined(__ICC) && !defined(__TMS470__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 6 || (__GNUC_MINOR__ == 6 && __GNUC_PATCHLEVEL__ > 0)))
#pragma GCC diagnostic push
#endif
#if !defined(__clang__)
#pragma GCC diagnostic ignored "-Wpragmas"
#endif
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wduplicate-decl-specifier"
#endif

void Mockspi_interface_Init(void);
void Mockspi_interface_Destroy(void);
void Mockspi_interface_Verify(void);




#define spi_enable_device_ExpectAndReturn(dev, pin_config, cmock_retval) spi_enable_device_CMockExpectAndReturn(__LINE__, dev, pin_config, cmock_retval)
void spi_enable_device_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, SPI_Device dev, SPI_PinConfig pin_config, SPI_Err cmock_to_return);
#define spi_disable_device_ExpectAndReturn(dev, cmock_retval) spi_disable_device_CMockExpectAndReturn(__LINE__, dev, cmock_retval)
void spi_disable_device_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, SPI_Device dev, SPI_Err cmock_to_return);
#define spi_reset_device_ExpectAndReturn(dev, cmock_retval) spi_reset_device_CMockExpectAndReturn(__LINE__, dev, cmock_retval)
void spi_reset_device_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, SPI_Device dev, SPI_Err cmock_to_return);
#define spi_configure_device_ExpectAndReturn(dev, config, cmock_retval) spi_configure_device_CMockExpectAndReturn(__LINE__, dev, config, cmock_retval)
void spi_configure_device_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, SPI_Device dev, SPI_Config config, SPI_Err cmock_to_return);
#define spi_write_Expect(dev, data) spi_write_CMockExpect(__LINE__, dev, data)
void spi_write_CMockExpect(UNITY_LINE_TYPE cmock_line, SPI_Device dev, uint16_t data);
#define spi_read_ExpectAndReturn(dev, cmock_retval) spi_read_CMockExpectAndReturn(__LINE__, dev, cmock_retval)
void spi_read_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, SPI_Device dev, uint16_t cmock_to_return);
#define spi_is_enabled_ExpectAndReturn(dev, cmock_retval) spi_is_enabled_CMockExpectAndReturn(__LINE__, dev, cmock_retval)
void spi_is_enabled_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, SPI_Device dev, bool cmock_to_return);
#define spi_is_configured_ExpectAndReturn(dev, cmock_retval) spi_is_configured_CMockExpectAndReturn(__LINE__, dev, cmock_retval)
void spi_is_configured_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, SPI_Device dev, bool cmock_to_return);
#define spi_packet_size_ExpectAndReturn(dev, cmock_retval) spi_packet_size_CMockExpectAndReturn(__LINE__, dev, cmock_retval)
void spi_packet_size_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, SPI_Device dev, uint8_t cmock_to_return);
#define spi_wait_tx_Expect(dev) spi_wait_tx_CMockExpect(__LINE__, dev)
void spi_wait_tx_CMockExpect(UNITY_LINE_TYPE cmock_line, SPI_Device dev);

#if defined(__GNUC__) && !defined(__ICC) && !defined(__TMS470__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 6 || (__GNUC_MINOR__ == 6 && __GNUC_PATCHLEVEL__ > 0)))
#pragma GCC diagnostic pop
#endif
#endif

#endif
