#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "msgq.h"
#include "usb.h"

void msq_lock(void)
{
  // do nothing for now
}

void msq_unlock(void)
{
  // do nothing for now
}

void wait_us(uint32_t t)
{
  usleep(t);
}

void hexdump(char c[], int size)
{
  for (int i = 0; i < size; i = i + 4)
    printf("%02x %02x %02x %02x\n", c[i], c[i + 1], c[i + 2], c[i + 3]);
}

typedef struct __attribute__((packed))
{
  uint8_t header;
  uint16_t nesto;
  uint32_t nestoDrugo;
  uint8_t taman;
} TestMsg;

void test_enqueue_dequeue_one(void)
{
  MSG_queue q;
  msgq_init(&q);

  TestMsg tmsg = {.header = 0xde, .nesto = 0xadbe, .nestoDrugo = 0xffc0ffee, .taman = 0xaa};
  msgq_enqueue(&q, &tmsg, sizeof(TestMsg), false);

  printf("%lu\n", sizeof(TestMsg));
  assert(q.counter == 1);
  assert(q.current_msg == 0);
  printf("q.bottom_msg: %d\n", q.bottom_msg);
  assert(q.bottom_msg == 1);

  char buff[8];
  msgq_dequeue(&q, buff, false);
  assert(((TestMsg *)buff)->header == tmsg.header);
  assert(((TestMsg *)buff)->nesto == tmsg.nesto);
  assert(((TestMsg *)buff)->nestoDrugo == tmsg.nestoDrugo);
  assert(((TestMsg *)buff)->taman == tmsg.taman);
}

void test_enqueue_full(void)
{
  MSG_queue q;
  msgq_init(&q);

  char msg[8];
  for (int i = 0; i < 100; i++) {
    msg[0] = i + 1;
    memset(&msg[1], 'A', 7);
    msgq_enqueue(&q, msg, sizeof(msg), false);
  }

  assert(q.counter == 100);
  assert(q.current_msg == 0);
  assert(q.bottom_msg == 0);

  assert(msgq_enqueue(&q, msg, 8, false) == 0);

  char buff[8];
  for (int i = 0; i < 100; i++) {
    msgq_dequeue(&q, buff, false);
    assert(buff[0] == (i + 1));
  }

  assert(q.counter == 0);
  assert(q.current_msg == 0);
}

void test_dequeue_nonblock_empty(void)
{
  MSG_queue q;
  msgq_init(&q);

  char buff[8];

  assert(msgq_dequeue(&q, buff, false) == 0);
}

void test_enqueue_dequeue_usbsetup(void)
{
  MSG_queue q;
  msgq_init(&q);

  DescriptorPacket sp = {
    .bLength = 0x12,          // 8
    .bDescriptorType = 0x01,  // 8
    .bcdUSB = 0x01,           // 16
    .bDeviceClass = 0x02,     // 8
    .bDeviceSubClass = 0x00,  // 8
    .bDeviceProtocol = 0x00,  // 8
    .bMaxPacketSize0 = 64,    // 8
    .idVendor = 0x6976,
    /*iv*/  // 16
    .idProduct = 0x616e,
    /*an*/                       // 16
    .bcdDevice = 0x01,           // 16
    .iProduct = 0x02,            // 8
    .iSerialNumber = 0x03,       // 8
    .bNumConfigurations = 0x01,  // 8
  };

  msgq_enqueue(&q, &sp, sizeof(sp), false);

  char buff[sizeof(sp)] = {0};
  int sz = msgq_dequeue(&q, &buff[0], false);
  assert(sz == 8);

  sz = msgq_dequeue(&q, &buff[8], false);
  assert(sz == 8);

  sz = msgq_dequeue(&q, &buff[16], false);
  assert(sz == 2);

  hexdump(buff, sizeof(sp));

  assert(((DescriptorPacket *)buff)->bLength == sp.bLength);
  assert(((DescriptorPacket *)buff)->bDescriptorType == sp.bDescriptorType);
  assert(((DescriptorPacket *)buff)->bcdUSB == sp.bcdUSB);
  assert(((DescriptorPacket *)buff)->bDeviceClass == sp.bDeviceClass);
  assert(((DescriptorPacket *)buff)->bDeviceSubClass == sp.bDeviceSubClass);
  assert(((DescriptorPacket *)buff)->bDeviceProtocol == sp.bDeviceProtocol);
  assert(((DescriptorPacket *)buff)->bMaxPacketSize0 == sp.bMaxPacketSize0);
  assert(((DescriptorPacket *)buff)->idVendor == sp.idVendor);
  assert(((DescriptorPacket *)buff)->idProduct == sp.idProduct);
  assert(((DescriptorPacket *)buff)->bcdDevice == sp.bcdDevice);
  assert(((DescriptorPacket *)buff)->iProduct == sp.iProduct);
  assert(((DescriptorPacket *)buff)->iSerialNumber == sp.iSerialNumber);
  assert(((DescriptorPacket *)buff)->bNumConfigurations == sp.bNumConfigurations);
}

/* TODO tests for blocking cases */

int main()
{
  /* test_enqueue_dequeue_one(); */
  /* test_enqueue_full(); */
  /* test_dequeue_nonblock_empty(); */

  test_enqueue_dequeue_usbsetup();
  return 0;
}
