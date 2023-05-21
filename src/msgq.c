#include "msgq.h"
#include <string.h>

void msgq_init(MSG_queue *q)
{
  if (!q)
    return;

  q->counter = 0;
  q->current_msg = 0;
  q->bottom_msg  = 0;

  memset(q->queue, 0, sizeof(MSG_block) * MSGQ_NOMSG);
}

size_t msgq_enqueue(MSG_queue *q, void *msg, size_t size, bool is_blocking)
{
  if (!q)
    return -1;

  if (is_blocking)
  {
    while (q->counter >= MSGQ_NOMSG)
      wait_us(100);      
  }

  else if (q->counter >= MSGQ_NOMSG)
    return 0;

  int msg_size = 8;
  int retval = 0;
  MSGQ_LOCK();
  while (size > 0 && q->counter < MSGQ_NOMSG)
  {
    MSG_block *ptr = &q->queue[q->bottom_msg];
    if (size < msg_size)
      msg_size = size;

    ptr->size = msg_size;
    memcpy(ptr->data, msg, msg_size);

    // TODO: make sure that we can't write buffer when it's full
    q->bottom_msg = (q->bottom_msg + 1) % MSGQ_NOMSG;
    q->counter++;
    msg = (void *)((uintptr_t) msg + msg_size);
    size -= msg_size;
    retval += msg_size;
  }
  MSGQ_UNLOCK();

  return retval;
}

size_t msgq_dequeue(MSG_queue *q, void *msg, bool is_blocking)
{
  if (!q)
    return -1;

  if (is_blocking)
  {
    while (!q->counter)
      wait_us(100);
  }

  else if (!q->counter)
    return 0;

  MSGQ_LOCK();
  MSG_block *ptr = &q->queue[q->current_msg];
  memcpy(msg, ptr->data, ptr->size);
  q->current_msg = (q->current_msg + 1) % MSGQ_NOMSG;  
  q->counter--;
  MSGQ_UNLOCK();

  return ptr->size;
}
