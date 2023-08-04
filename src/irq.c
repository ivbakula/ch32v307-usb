#include "irq.h"

#include <string.h>

#include "csr.h"

typedef struct
{
  volatile uint32_t ISR[8];
  volatile uint32_t IPR[8];
  volatile uint32_t ITHRESDR;
  volatile uint32_t RESERVED;
  volatile uint32_t CFGR;
  volatile uint32_t GISR;
  volatile uint8_t VTFIDR[4];
  uint8_t RESERVED0[12];
  volatile uint32_t VTFADDR[4];
  uint8_t RESERVED1[0x90];
  volatile uint32_t IENR[8];
  uint8_t RESERVED2[0x60];
  volatile uint32_t IRER[8];
  uint8_t RESERVED3[0x60];
  volatile uint32_t IPSR[8];
  uint8_t RESERVED4[0x60];
  volatile uint32_t IPRR[8];
  uint8_t RESERVED5[0x60];
  volatile uint32_t IACTR[8];
  uint8_t RESERVED6[0xE0];
  volatile uint8_t IPRIOR[256];
  uint8_t RESERVED7[0x810];
  volatile uint32_t SCTLR;
} PFIC_Regfile;

#define PFIC ((PFIC_Regfile *)0xE000E000)

__attribute__((section(".data"))) __irq_handler IRQ_interrupt_vector[256];
__attribute__((section(".data"))) __irq_handler IRQ_exception_vector[16];

void irq_register_interrupt_handler(Interrupt_IRQn n, __irq_handler h)
{
  if (n > 255)
    return;
  IRQ_interrupt_vector[n] = h;
}

void irq_deregister_interrupt_handler(Interrupt_IRQn n)
{
  if (n > 255)
    return;

  irq_disable_interrupts_g();
  IRQ_interrupt_vector[n] = 0;
  irq_enable_interrupts_g();
}

void irq_register_exception_handler(Exception_IRQn n, __irq_handler h)
{
  if (n < 256)
    IRQ_exception_vector[n] = h;
}

void irq_deregister_exception_handler(Exception_IRQn n)
{
  if (n < 256)
    IRQ_exception_vector[n] = 0;
}

void irq_enable_interrupt(Interrupt_IRQn irq)
{
  PFIC->IENR[((uint32_t)(irq) >> 5)] = (1 << ((uint32_t)(irq)&0x1f));
}

void irq_disable_interrupt(Interrupt_IRQn irq)
{
  PFIC->IRER[((uint32_t)(irq) >> 5)] = (1 << ((uint32_t)(irq)&0x1F));
}

uint32_t irq_get_interrupt_status(Interrupt_IRQn irq)
{
  return ((uint32_t)((PFIC->ISR[(uint32_t)(irq) >> 5] & (1 << ((uint32_t)(irq)&0x1F))) ? 1 : 0));
}

static inline void __trap_handle_interrupt(uint32_t irqn) __attribute__((always_inline));
static inline void __trap_handle_interrupt(uint32_t irqn)
{
  if (irqn < 256 && IRQ_interrupt_vector[irqn])
    IRQ_interrupt_vector[irqn]();
}

extern void red_light();
static inline void __trap_handle_exception(uint32_t excn) __attribute__((always_inline));
static inline void __trap_handle_exception(uint32_t excn)
{
  red_light();
  return;
}

/**
 * @fn irq_trap
 *
 * @brief Main interrupt and exception trap. Address of this function is
 *        loaded to the mtvec register, and it's called on every interrupt
 *        and exception. This function will than determine which interrupt
 *        or exception occured by reading mcause register. After interrupt
 *        code is determined, this function will call it's interrupt handler
 *        by consulting with IRQ_(interrupt|exception)_vector array
 */
void _irq_trap(void) __attribute__((interrupt));
void _irq_trap(void)
{
  CSR_mcause mcause = csr_read_mcause();
  switch (mcause.type) {
    case MCAUSE_TYPE_INTERRUPT:
      __trap_handle_interrupt(mcause.code);
      break;

    case MCAUSE_TYPE_EXCEPTION:
      __trap_handle_exception(mcause.code);
  }
}
