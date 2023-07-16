#ifndef MSGQ_H
#define MSGQ_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MSGQ_NOMSG 100 /* max number of messages in message queue */

typedef union
{
  struct
  {
    uint32_t size;
    uint8_t data[8];
  } __attribute__((__packed__));
  uint8_t memblk[12];
} MSG_block;

/**
 * @class MSG_queue
 *
 * @brief Message queues are used for passing "large" amounts of
 *        data from interrupt
 *        context. MSGQ supprots both blocking and non-blocking
 *        operations (enqueue/dequeue). Blocking operations
 *        are generally used in normal threads while waiting for
 *        an interrupt and non-blocking operations are used in
 *        interrupt routines.
 *
 *        On singe core cpu, it is really really unwise to use
 *        blocking operation in an interrupt routine, because
 *        you might end up waiting forever ;). To elaborate more,
 *        Interrupts will always preempt "normal" threads and
 *        if interrupt routine blocks for some event from "normal"
 *        thread (or even from an interrupt of lower priority) this
 *        event will never have a chance to occur and you will be
 *        stuck while waiting for Godot.
 */
typedef struct
{
  volatile uint8_t counter;
  uint32_t current_msg;
  uint32_t bottom_msg;
  MSG_block queue[MSGQ_NOMSG];
} MSG_queue __attribute__((aligned(4)));

/**
 * @fn msgq_init
 *
 * @brief Initialize message queue.
 *
 * @param q: pointer to allocated message queue
 */
void msgq_init(MSG_queue *q);

/**
 * @fn msgq_enqueue
 *
 * @brief Write message to the message queue
 *
 * @param q message queue structure
 * @param msg message
 * @param is_blocking
 */
size_t msgq_enqueue(MSG_queue *q, void *msg, size_t size, bool is_blocking);

/**
 * @fn msgq_dequeue
 *
 * @brief Read message from the message queue
 *
 * @param q message queue
 * @param msg Pointer to memory allocated for message. It needs to be atleast 8 bytes (for now)
 * @param is_blocking: true - dequeue operation should block (If message queue is empty, wait for
 *                     new message)
 *                     false - dequeue operation will not block (If message queue is empty, return
 *                     immidiatley)
 * @return number of bytes.
 */
size_t msgq_dequeue(MSG_queue *q, void *msg, bool is_blocking);

#ifndef UNITTEST
#include "irq.h"
#include "time.h"
#define MSGQ_LOCK(X)   irq_disable_interrupts_g()
#define MSGQ_UNLOCK(X) irq_enable_interrupts_g()
#else
#define MSGQ_LOCK(x)
#define MSGQ_UNLOCK(x)
#include <unistd.h>

void wait_us(uint32_t us);

#endif

#endif
