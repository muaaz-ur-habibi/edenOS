#include "idt.h"

idt_gate_data idt[IDT_N];
idt_register_data idt_reg;

void set_idt_gate(int n, uint32_t handler)
{
    idt[n].low_offset = low_16(handler);
    idt[n].high_offset = high_16(handler);
    idt[n].kernel_seg_sel = KERNEL_SEG_SEL;
    idt[n].always0 = 0;
    idt[n].flags = 0x8e;
}

void set_idt()
{
    idt_reg.base = (uint32_t) &idt;
    idt_reg.limit = sizeof(idt_gate_data) * IDT_N - 1;

    __asm__ __volatile__("lidtl (%0)" :: "r" (&idt_reg));
}