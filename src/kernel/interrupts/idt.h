#ifndef KIDT
#define KIDT

#include "../globals/types.h"
#include "../drivers/vga/vga.h"

#define KERNEL_SEG_SEL 0x08

typedef struct {
    uint16_t low_offset;
    uint16_t kernel_seg_sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t high_offset;
} __attribute__((packed)) idt_gate_data;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_register_data;

#define IDT_N 256
extern idt_gate_data idt[IDT_N];
extern idt_register_data idt_reg;

void set_idt_gate(int n, uint32_t handler);
void set_idt();

#endif