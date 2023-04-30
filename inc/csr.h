#ifndef CSR_H
#define CSR_H

#include <stdint.h>

typedef union
{
  struct
  {
    uint32_t code : 31;
    uint32_t type : 1;
  };
  uint32_t data;
} CSR_mcause;

enum
{
  MCAUSE_TYPE_EXCEPTION = 0,
  MCAUSE_TYPE_INTERRUPT,
};

typedef union
{
  struct
  {
    uint32_t reserved0 : 3;
    uint32_t MIE       : 1;  /* Machine mode  Interrupt Enable */
    uint32_t reserved1 : 3;
    uint32_t MPIE      : 1;  /* Interrupt enable state before entrering interrupt */
    uint32_t reserved3 : 3;
    uint32_t MPP       : 2;  /* Privilege mode before entering interrupt */
    uint32_t FPUS      : 2;  /* Floating point unit status */
    uint32_t reserved4 : 16;
  };
  uint32_t data;  
} CSR_mstatus;

typedef union
{
  struct
  {
    uint32_t HWSTKEN : 1; /* Enable HW stack.
                           * 0b0: HPE function is off
                           * 0b1: HPE function is on */

    uint32_t INESTEN : 1; /* Interrupt nesting enable
                           * 0b0: Interrupt nesting is off
                           * 0b1: Interrupt nesting is on */

    uint32_t PMTCFG : 2; /* Interrupt nesting depth config:
                          * 0b00: No nesting, the number of preemption bits is 0
                          * 0b01: 2 levels of nesting, with 1 preemption bit
                          * 0b10: 4 levels of nesting, with 2 preemption bits
                          * 0b11: 8 levels of nesting, with 3 preemption bits */
    
    uint32_t HWSTKOVEN : 1; /* Interrupt enable after HPE (Hardware stack)
                            * overflow. 0b0 global interrupts are turned off
                            * after HPE overflow 0b1 Interrupts are stil turned
                            * on after HPE overflow. */

    uint32_t GIHWSTKNEN : 1; /* Disable global interrupt and HPE. Set this bit
                              * to turn off the global interrupt and HPE. This
                              * bit has meaning only in interrupt context (it
                              * prevents nested interrupts) and it's cleared
                              * when interrupt routine returns */

    uint32_t reserved0 : 2;

    uint32_t PMTSTA : 8; /* Preemption status indication.
                         * 0x00: No preemption bits in the priority
                         * configuration bits. No interrupt nesting 0x80: The
                         * highest bit in the priority configuration is a
                         * preemption bit. Interrupt nesting is enabled, with 2
                         * levels of nesting */

    uint32_t reserved1 : 16;
  };
  uint32_t data;
} CSR_Intsyscr;
    
static inline CSR_mcause csr_read_mcause (void)
{
  CSR_mcause mcause;
  __asm__ volatile("csrr %0, mcause"
                   : "=r"(mcause.data) /* output : register */
                   :                   /* input : none */
                   :                   /* clobbers : none */
  );

  return mcause;
}

static inline void csr_write_mcause (CSR_mcause mcause)
{
  __asm__ volatile("csrw mcause, %0"
                   :                  /* output : none */
                   : "r"(mcause.data) /* input : from register */
                   :                  /* clobbers : none */
  );
}

static inline CSR_mstatus csr_read_mstatus (void)
{
  CSR_mstatus mstatus = {.data = 0};
  __asm__ volatile("csrr %0, mstatus"
                   : "=r"(mstatus.data) /* output : register */
                   :                    /* input : none */
                   :                    /* clobbers : none */
  );

  return mstatus;
}

static inline void csr_write_mstatus (CSR_mstatus mstatus)
{
  __asm__ volatile("csrw mstatus, %0"
                   :                   /* output : none */
                   : "r"(mstatus.data) /* input : from register */
                   :                   /* clobbers : none */
  );
  
}

static inline CSR_Intsyscr csr_read_intsyscr(void)
{
  CSR_Intsyscr intsyscr = {.data = 0};
  __asm__ volatile("csrr %0, 0x804"
                   : "=r"(intsyscr.data) /* output : register */
                   :                     /* input : none */
                   :                     /* clobbers : none */
  );

  return intsyscr;
}

static inline void csr_write_intsyscr(CSR_Intsyscr intsyscr)
{
  __asm__ volatile("csrw 0x804, %0"
                   :                    /* output : register */
                   : "r"(intsyscr.data) /* input : none */
                   :                    /* clobbers : none */
  );
}

#endif
