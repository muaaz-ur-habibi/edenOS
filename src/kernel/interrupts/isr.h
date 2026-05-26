#ifndef KISR
#define KISR

#include "idt.h"
#include "../drivers/vga/vga.h"

typedef struct {
    unsigned int ds;
    unsigned int edi, esi, ebp, useless, ebx, edx, ecx, eax;
    unsigned int interrupt_no, error_code;
    unsigned int eip, cs, eflags, esp, ss;
} registers_data;

typedef void (*isr_data)(registers_data*);

extern void isr_div0(void);
extern void isr_dbg(void);
extern void isr_non_mskble(void);
extern void isr_int3(void);
extern void isr_into(void);
extern void isr_oob(void);
extern void isr_opc(void);
extern void isr_coproc(void);
extern void isr_2fault(void);
extern void isr_coproc_segoverrun(void);
extern void isr_bad_tss(void);
extern void isr_seg_unav(void);
extern void isr_stack_fault(void);
extern void isr_gen_protec_fault(void);
extern void isr_page_fault(void);
extern void isr_rsrv_e(void);
extern void isr_float(void);
extern void isr_align_chck(void);
extern void isr_mach_chck(void);

extern void isr_rsrv1(void);
extern void isr_rsrv2(void);
extern void isr_rsrv3(void);
extern void isr_rsrv4(void);
extern void isr_rsrv5(void);
extern void isr_rsrv6(void);
extern void isr_rsrv7(void);
extern void isr_rsrv8(void);
extern void isr_rsrv9(void);
extern void isr_rsrv10(void);
extern void isr_rsrv11(void);
extern void isr_rsrv12(void);
extern void isr_rsrv13(void);

extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);

#define IRQ0  32
#define IRQ1  33
#define IRQ2  34
#define IRQ3  35
#define IRQ4  36
#define IRQ5  37
#define IRQ6  38
#define IRQ7  39
#define IRQ8  40
#define IRQ9  41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

void isr_install_handlers();
void isr_handler(registers_data *reg);
void register_interrupt_handler(uint8_t n, isr_data handler);

#endif