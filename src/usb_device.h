#ifndef USB_DEVICE_H
#define USB_DEVICE_H

#include <stddef.h>
#include <stdint.h>

#include "usb_interface.h"

#define TOKEN_MASK  ((uint8_t)(0b11 << 4))
#define TOKEN_OUT   ((uint8_t)(0b00 << 4))
#define TOKEN_SOF   ((uint8_t)(0b01 << 4))
#define TOKEN_IN    ((uint8_t)(0b10 << 4))
#define TOKEN_SETUP ((uint8_t)((0b11 << 4)))

typedef enum
{
  EP_IDLE,
  EP_TX_IN_PROGRESS,
  EP_RX_IN_PROGRESS,
  EP_TX_DONE,
  EP_RX_DONE,
} EP_Status;

struct USB_Ctrl_Endpoint
{
  uint8_t rx_dma_buffer[8];

  size_t packet_size;
  size_t tx_data_left;
  EP_Status status;
};

typedef enum
{
  USBD_UNINITIALIZED,
  USBD_INITIALIZED,
  USBD_DESCRIPTOR_EXCHANGE,
  USBD_ADDRESS_ASSIGNEMENT,
  USBD_CONFIGURATION_EXCHANGE,
  USBD_ENUM_DONE,
  USBD_READY
} USBD_State;

struct USB_Device
{
  volatile USBD_State state;
  size_t max_packet_size_ctrl; /* Max USB packet size for control transfers   */
  size_t max_packet_size_irq;  /* Max USB packet size for interrupt transfers */

  struct USB_Ctrl_Endpoint ep0;
};

typedef struct
{
  /* STOLEN from ch32v307 repo */
  volatile uint8_t CONTROL;
  volatile uint8_t HOST_CTRL;
  volatile uint8_t INT_EN;
  volatile uint8_t DEV_AD;
  volatile uint16_t FRAME_NO;
  volatile uint8_t SUSPEND;
  volatile uint8_t RESERVED0;
  volatile uint8_t SPEED_TYPE;
  volatile uint8_t MIS_ST;
  volatile uint8_t INT_FG;
  volatile uint8_t INT_ST;
  volatile uint16_t RX_LEN;
  volatile uint16_t RESERVED1;
  volatile uint32_t ENDP_CONFIG;
  volatile uint32_t ENDP_TYPE;
  volatile uint32_t BUF_MODE;
  volatile uint32_t UEP0_DMA;
  volatile uint32_t UEP1_RX_DMA;
  volatile uint32_t UEP2_RX_DMA;
  volatile uint32_t UEP3_RX_DMA;
  volatile uint32_t UEP4_RX_DMA;
  volatile uint32_t UEP5_RX_DMA;
  volatile uint32_t UEP6_RX_DMA;
  volatile uint32_t UEP7_RX_DMA;
  volatile uint32_t UEP8_RX_DMA;
  volatile uint32_t UEP9_RX_DMA;
  volatile uint32_t UEP10_RX_DMA;
  volatile uint32_t UEP11_RX_DMA;
  volatile uint32_t UEP12_RX_DMA;
  volatile uint32_t UEP13_RX_DMA;
  volatile uint32_t UEP14_RX_DMA;
  volatile uint32_t UEP15_RX_DMA;
  volatile uint32_t UEP1_TX_DMA;
  volatile uint32_t UEP2_TX_DMA;
  volatile uint32_t UEP3_TX_DMA;
  volatile uint32_t UEP4_TX_DMA;
  volatile uint32_t UEP5_TX_DMA;
  volatile uint32_t UEP6_TX_DMA;
  volatile uint32_t UEP7_TX_DMA;
  volatile uint32_t UEP8_TX_DMA;
  volatile uint32_t UEP9_TX_DMA;
  volatile uint32_t UEP10_TX_DMA;
  volatile uint32_t UEP11_TX_DMA;
  volatile uint32_t UEP12_TX_DMA;
  volatile uint32_t UEP13_TX_DMA;
  volatile uint32_t UEP14_TX_DMA;
  volatile uint32_t UEP15_TX_DMA;
  volatile uint16_t UEP0_MAX_LEN;
  volatile uint16_t RESERVED2;
  volatile uint16_t UEP1_MAX_LEN;
  volatile uint16_t RESERVED3;
  volatile uint16_t UEP2_MAX_LEN;
  volatile uint16_t RESERVED4;
  volatile uint16_t UEP3_MAX_LEN;
  volatile uint16_t RESERVED5;
  volatile uint16_t UEP4_MAX_LEN;
  volatile uint16_t RESERVED6;
  volatile uint16_t UEP5_MAX_LEN;
  volatile uint16_t RESERVED7;
  volatile uint16_t UEP6_MAX_LEN;
  volatile uint16_t RESERVED8;
  volatile uint16_t UEP7_MAX_LEN;
  volatile uint16_t RESERVED9;
  volatile uint16_t UEP8_MAX_LEN;
  volatile uint16_t RESERVED10;
  volatile uint16_t UEP9_MAX_LEN;
  volatile uint16_t RESERVED11;
  volatile uint16_t UEP10_MAX_LEN;
  volatile uint16_t RESERVED12;
  volatile uint16_t UEP11_MAX_LEN;
  volatile uint16_t RESERVED13;
  volatile uint16_t UEP12_MAX_LEN;
  volatile uint16_t RESERVED14;
  volatile uint16_t UEP13_MAX_LEN;
  volatile uint16_t RESERVED15;
  volatile uint16_t UEP14_MAX_LEN;
  volatile uint16_t RESERVED16;
  volatile uint16_t UEP15_MAX_LEN;
  volatile uint16_t RESERVED17;
  volatile uint16_t UEP0_TX_LEN;
  volatile uint8_t UEP0_TX_CTRL;
  volatile uint8_t UEP0_RX_CTRL;
  volatile uint16_t UEP1_TX_LEN;
  volatile uint8_t UEP1_TX_CTRL;
  volatile uint8_t UEP1_RX_CTRL;
  volatile uint16_t UEP2_TX_LEN;
  volatile uint8_t UEP2_TX_CTRL;
  volatile uint8_t UEP2_RX_CTRL;
  volatile uint16_t UEP3_TX_LEN;
  volatile uint8_t UEP3_TX_CTRL;
  volatile uint8_t UEP3_RX_CTRL;
  volatile uint16_t UEP4_TX_LEN;
  volatile uint8_t UEP4_TX_CTRL;
  volatile uint8_t UEP4_RX_CTRL;
  volatile uint16_t UEP5_TX_LEN;
  volatile uint8_t UEP5_TX_CTRL;
  volatile uint8_t UEP5_RX_CTRL;
  volatile uint16_t UEP6_TX_LEN;
  volatile uint8_t UEP6_TX_CTRL;
  volatile uint8_t UEP6_RX_CTRL;
  volatile uint16_t UEP7_TX_LEN;
  volatile uint8_t UEP7_TX_CTRL;
  volatile uint8_t UEP7_RX_CTRL;
  volatile uint16_t UEP8_TX_LEN;
  volatile uint8_t UEP8_TX_CTRL;
  volatile uint8_t UEP8_RX_CTRL;
  volatile uint16_t UEP9_TX_LEN;
  volatile uint8_t UEP9_TX_CTRL;
  volatile uint8_t UEP9_RX_CTRL;
  volatile uint16_t UEP10_TX_LEN;
  volatile uint8_t UEP10_TX_CTRL;
  volatile uint8_t UEP10_RX_CTRL;
  volatile uint16_t UEP11_TX_LEN;
  volatile uint8_t UEP11_TX_CTRL;
  volatile uint8_t UEP11_RX_CTRL;
  volatile uint16_t UEP12_TX_LEN;
  volatile uint8_t UEP12_TX_CTRL;
  volatile uint8_t UEP12_RX_CTRL;
  volatile uint16_t UEP13_TX_LEN;
  volatile uint8_t UEP13_TX_CTRL;
  volatile uint8_t UEP13_RX_CTRL;
  volatile uint16_t UEP14_TX_LEN;
  volatile uint8_t UEP14_TX_CTRL;
  volatile uint8_t UEP14_RX_CTRL;
  volatile uint16_t UEP15_TX_LEN;
  volatile uint8_t UEP15_TX_CTRL;
  volatile uint8_t UEP15_RX_CTRL;
} USBHD_Regfile;

#define USBHD_BASE ((uint32_t)0x40023400)
#define USBHD      ((USBHD_Regfile *)USBHD_BASE)

/* See table RCC_CFGR2 register description (pg 50 in manual) */
#define USB_HSSRC_USBPHY (U32_BIT(31))
#define USB_HSCLK_3MHz   (0b00 << 29)
#define USB_HSCLK_4MHz   (0b01 << 29)
#define USB_HSCLK_8MHz   (0b10 << 29)
#define USB_HSCLK_5MHz   (0b11 << 29)
#define USB_HSPLL_EN     (U32_BIT(30))

#define USBHD_CONTROL_DMA_EN        (U8_BIT(0))
#define USBHD_CONTROL_CLR_ALL       (U8_BIT(1))
#define USBHD_CONTROL_RESET_SIE     (U8_BIT(2))
#define USBHD_CONTROL_BSY           (U8_BIT(3))
#define USBHD_CONTROL_PUEN          (U8_BIT(4))
#define USBHD_CONTROL_SPEED_TYPE(x) ((x) << 5)

#define USBHD_INTEN_UIEDETECT (U8_BIT(0)) /*< Host mode interrupt: USB device connect/disconnect event */
#define USBHD_INTEN_BUSRST    (U8_BIT(0)) /*< Device mode interrupt: Bus reset event */
#define USBHD_INTEN_TRANSFER  (U8_BIT(1)) /*< Transfer complete interrupt */
#define USBHD_INTEN_SUSPEND   (U8_BIT(2)) /*< Suspend event interrupt */

/**
 * USBHD_INTEN_SOFACT: SOF timing interrupt in host mode;
 * In device mode, receive SOF packet to generate Transfer complete interrupt
 */
#define USBHD_INTEN_SOFACT (U8_BIT(3))
#define USBHD_INTEN_FIFO   (U8_BIT(4)) /*< Fifo overflow interrupt */
#define USBHD_INTEN_SETUP  (U8_BIT(5)) /*< Setup transfer complete */
#define USBHD_INTEN_ISO    (U8_BIT(6)) /*< ISO transfer complete event */
#define USBHD_INTEN_NAK    (U8_BIT(7)) /*< Device mode: NAK interrupt */

#define USBHD_TXCTRL_RES_NAK 0x2

#define USBHD_HOST_CTRL_PHY_SUSPEND (U8_BIT(4))

#define USBHD_INTFG_BUSRST  (U8_BIT(0)) /*< USBD: bus reset event;     */
#define USBHD_INTFG_UIF     (U8_BIT(0)) /*< USBH: device connect event */
#define USBHD_INTFG_TRANS   (U8_BIT(1)) /*< Transfer complete event    */
#define USBHD_INTFG_SUSPEND (U8_BIT(2)) /*< Suspend/wake-up event      */
#define USBHD_INTFG_SOF     (U8_BIT(3)) /*< SOF timer interrupt flag   */
#define USBHD_INTFG_FIFO    (U8_BIT(4)) /*< USB FIFO overflow interrupt*/
#define USBHD_INTFG_SETUP   (U8_BIT(5)) /*< SETUP transaction complete */
#define USBHD_INTFG_ISO_ACT (U8_BIT(6)) /*< Sync transmission start send/recv */

#endif
