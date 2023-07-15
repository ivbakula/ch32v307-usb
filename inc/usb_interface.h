#ifndef USB_INTERFACE_H
#define USB_INTERFACE_H

#include <stdint.h>
#include <stddef.h>

typedef enum
{
  USB_FULL_SPEED = 0,
  USB_HIGH_SPEED = 1,
  USB_LOW_SPEED = 2,
} USB_DeviceMode;

/**
 * @fn init_usb_device
 *
 * @brief Initializes USB hardware and USB endpoints.
 *
 * @param mode: USB device speed mode: USB_FULL_SPEED|USB_HIGH_SPEED|USB_LOW_SPEED
 */
void init_usb_device(USB_DeviceMode mode);

size_t read_endpoint(char *buffer, size_t buffer_sz, int endpoint);

void write_endpoint(char *buffer, size_t buffer_sz, int endpoint);

enum
{
  DEVICE_DESCRIPTOR = 1,
  CONFIGURATION_DESCRIPTOR,
  STRING_DESCRIPTOR,
  INTERFACE_DESCRIPTOR,
  ENDPOINT_DESCRIPTOR,
};

struct SetupPacket
{
  uint8_t bmRequestType;
  uint8_t bRequest;
  uint16_t wValue;
  uint16_t wIndex;
  uint16_t wLength;
} __attribute__((packed));

typedef struct SetupPacket SetupPacket;

struct DeviceDescriptor
{
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint16_t bcdUSB;
  uint8_t bDeviceClass;
  uint8_t bDeviceSubClass;
  uint8_t bDeviceProtocol;
  uint8_t bMaxPacketSize0;
  uint16_t idVendor;
  uint16_t idProduct;
  uint16_t bcdDevice;
  uint8_t iManufacturer;
  uint8_t iProduct;
  uint8_t iSerialNumber;
  uint8_t bNumConfigurations;
} __attribute__((packed));
typedef struct DeviceDescriptor DeviceDescriptor;

struct ConfigurationDescriptor
{
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint16_t wTotalLength;
  uint8_t bNumInterfaces;
  uint8_t bConfigurationValue;
  uint8_t iConfiguration;
  uint8_t bmAttributes;
  uint8_t bMaxPower;
} __attribute__((packed));
typedef struct ConfigurationDescriptor ConfigurationDescriptor;

struct InterfaceDescriptor
{
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bInterfaceNumber;
  uint8_t bAlternateSetting;
  uint8_t bNumEndpoints;
  uint8_t bInterfaceClass;
  uint8_t bInterfaceSubClass;
  uint8_t bInterfaceProtocol;
  uint8_t iInterface;
} __attribute__((packed));
typedef struct InterfaceDescriptor InterfaceDescriptor;

struct EndpointDescriptor
{
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint8_t bEndpointAddress;
  uint8_t bmAttributes;
  uint16_t wMaxPacketSize;
  uint8_t bInterval;
} __attribute__((packed));
typedef struct EndpointDescriptor EndpointDescriptor;

struct StringDescriptorComm
{
  uint8_t bLength;
  uint8_t bDescriptorType;
} __attribute__((packed));
typedef struct StringDescriptorComm StringDescriptorComm;

typedef enum
{
  GET_STATUS = 0,
  CLEAR_FEATURE,
  SET_FEATURE = 3,
  SET_ADDRESS = 5,
  GET_DESCRIPTOR,
  SET_DESCRIPTOR,
  GET_CONFIGURATION,
  SET_CONFIGURATION
} USB_bRequest;

#endif
