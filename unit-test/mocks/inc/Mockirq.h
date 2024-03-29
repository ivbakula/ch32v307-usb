/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCKIRQ_H
#define _MOCKIRQ_H

#include "unity.h"
#include "irq.h"

/* Ignore the following warnings, since we are copying code */
#if defined(__GNUC__) && !defined(__ICC) && !defined(__TMS470__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 6 || (__GNUC_MINOR__ == 6 && __GNUC_PATCHLEVEL__ > 0)))
#pragma GCC diagnostic push
#endif
#if !defined(__clang__)
#pragma GCC diagnostic ignored "-Wpragmas"
#endif
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wduplicate-decl-specifier"
#endif

void Mockirq_Init(void);
void Mockirq_Destroy(void);
void Mockirq_Verify(void);




#define irq_register_interrupt_handler_Expect(n, h) irq_register_interrupt_handler_CMockExpect(__LINE__, n, h)
void irq_register_interrupt_handler_CMockExpect(UNITY_LINE_TYPE cmock_line, Interrupt_IRQn n, __irq_handler h);
#define irq_deregister_interrupt_handler_Expect(n) irq_deregister_interrupt_handler_CMockExpect(__LINE__, n)
void irq_deregister_interrupt_handler_CMockExpect(UNITY_LINE_TYPE cmock_line, Interrupt_IRQn n);
#define irq_register_exception_handler_Expect(n, h) irq_register_exception_handler_CMockExpect(__LINE__, n, h)
void irq_register_exception_handler_CMockExpect(UNITY_LINE_TYPE cmock_line, Exception_IRQn n, __irq_handler h);
#define irq_deregister_exception_handler_Expect(n) irq_deregister_exception_handler_CMockExpect(__LINE__, n)
void irq_deregister_exception_handler_CMockExpect(UNITY_LINE_TYPE cmock_line, Exception_IRQn n);
#define irq_enable_interrupt_Expect(irq) irq_enable_interrupt_CMockExpect(__LINE__, irq)
void irq_enable_interrupt_CMockExpect(UNITY_LINE_TYPE cmock_line, Interrupt_IRQn irq);
#define irq_get_interrupt_status_ExpectAndReturn(irq, cmock_retval) irq_get_interrupt_status_CMockExpectAndReturn(__LINE__, irq, cmock_retval)
void irq_get_interrupt_status_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, Interrupt_IRQn irq, uint32_t cmock_to_return);
#define irq_disable_interrupt_Expect(irq) irq_disable_interrupt_CMockExpect(__LINE__, irq)
void irq_disable_interrupt_CMockExpect(UNITY_LINE_TYPE cmock_line, Interrupt_IRQn irq);

#if defined(__GNUC__) && !defined(__ICC) && !defined(__TMS470__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 6 || (__GNUC_MINOR__ == 6 && __GNUC_PATCHLEVEL__ > 0)))
#pragma GCC diagnostic pop
#endif
#endif

#endif
