#ifndef INC_USBHD_INTERFACE_H
#define INC_USBHD_INTERFACE_H

typedef enum
{
  USB_Err_Success,
} USB_Err;

USB_Err usbhd_enable_device(void);

#endif
