#ifndef _USB_H
#define _USB_H

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

struct CompoundDescriptor
{
  ConfigurationDescriptor config_descr;
  InterfaceDescriptor iface_descr;
  EndpointDescriptor ep_descr;
  InterfaceDescriptor iface_descr2;
  EndpointDescriptor ep_descr2;
} __attribute__((packed));
typedef struct CompoundDescriptor CompoundDescriptor;

struct StringDescriptor
{
  uint8_t bLength;
  uint8_t bDescriptorType;
  uint16_t wLANGID[1];
} __attribute__((packed));
typedef struct StringDescriptor StringDescriptor;

struct USB_DescriptorString
{
  uint8_t bLength;
  uint8_t bDescriptorType;
  char bString[];
} __attribute__((packed));
typedef struct USB_DescriptorString USB_DescriptorString;

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

size_t usb_setup_read_ep0(void *buffer);
size_t usb_setup_write_ep0(void *buffer, int size_left);
void usb_assign_address(uint8_t address);

#define TOKEN_MASK  ((uint8_t)(0b11 << 4))
#define TOKEN_OUT   ((uint8_t)(0b00 << 4))
#define TOKEN_SOF   ((uint8_t)(0b01 << 4))
#define TOKEN_IN    ((uint8_t)(0b10 << 4))
#define TOKEN_SETUP ((uint8_t)((0b11 << 4)))

#endif
