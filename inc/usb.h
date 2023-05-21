#ifndef _USB_H
#define _USB_H

struct SetupPacket {
  uint8_t bmRequestType;
  uint8_t bRequest;
  uint8_t wValue;
  uint8_t wIndex;
  uint16_t wLength;
} __attribute__((packed));

typedef struct SetupPacket SetupPacket;

struct DescriptorPacket {
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

typedef struct DescriptorPacket DescriptorPacket;

typedef enum {
  GET_STATUS = 0,
  CLEAR_FEATURE,
  SET_FEATURE,
  SET_ADDRESS,
  GET_DESCRIPTOR,
  SET_DESCRIPTOR,
  GET_CONFIGURATION,
  SET_CONFIGURATION
} USB_bRequest;

size_t usb_setup_read_ep0(void *buffer);
size_t usb_setup_write_ep0(void *buffer, int size_left);
void usb_assign_address(uint8_t address);

#define TOKEN_MASK ((uint8_t)(0b11 << 4))
#define TOKEN_OUT ((uint8_t)(0b00 << 4))
#define TOKEN_SOF ((uint8_t)(0b01 << 4))
#define TOKEN_IN ((uint8_t)(0b10 << 4))
#define TOKEN_SETUP ((uint8_t)((0b11 << 4)))

#endif
