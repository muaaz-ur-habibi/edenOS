#include "isr.h"

isr_data interrupt_handlers[256];

void isr_install_handlers()
{
    // install the isr's 
    set_idt_gate(0,  (uint32_t)isr_div0);
    set_idt_gate(1,  (uint32_t)isr_dbg);
    set_idt_gate(2,  (uint32_t)isr_non_mskble);
    set_idt_gate(3,  (uint32_t)isr_int3);
    set_idt_gate(4,  (uint32_t)isr_into);
    set_idt_gate(5,  (uint32_t)isr_oob);
    set_idt_gate(6,  (uint32_t)isr_opc);
    set_idt_gate(7,  (uint32_t)isr_coproc);
    set_idt_gate(8,  (uint32_t)isr_2fault);
    set_idt_gate(9,  (uint32_t)isr_coproc_segoverrun);
    set_idt_gate(10, (uint32_t)isr_bad_tss);
    set_idt_gate(11, (uint32_t)isr_seg_unav);
    set_idt_gate(12, (uint32_t)isr_stack_fault);
    set_idt_gate(13, (uint32_t)isr_gen_protec_fault);
    set_idt_gate(14, (uint32_t)isr_page_fault);
    set_idt_gate(15, (uint32_t)isr_rsrv_e);
    set_idt_gate(16, (uint32_t)isr_float);
    set_idt_gate(17, (uint32_t)isr_align_chck);
    set_idt_gate(18, (uint32_t)isr_mach_chck);
    set_idt_gate(19, (uint32_t)isr_rsrv1);
    set_idt_gate(20, (uint32_t)isr_rsrv2);
    set_idt_gate(21, (uint32_t)isr_rsrv3);
    set_idt_gate(22, (uint32_t)isr_rsrv4);
    set_idt_gate(23, (uint32_t)isr_rsrv5);
    set_idt_gate(24, (uint32_t)isr_rsrv6);
    set_idt_gate(25, (uint32_t)isr_rsrv7);
    set_idt_gate(26, (uint32_t)isr_rsrv8);
    set_idt_gate(27, (uint32_t)isr_rsrv9);
    set_idt_gate(28, (uint32_t)isr_rsrv10);
    set_idt_gate(29, (uint32_t)isr_rsrv11);
    set_idt_gate(30, (uint32_t)isr_rsrv12);
    set_idt_gate(31, (uint32_t)isr_rsrv13);

    // remap the programmable interrupt controller, since 0-31 are taken by isr's
    // its accessed using i/o ports
    port_byte_out(0x20, 0x11);
    port_byte_out(0xA0, 0x11);
    port_byte_out(0x21, 0x20);
    port_byte_out(0xA1, 0x28);
    port_byte_out(0x21, 0x04);
    port_byte_out(0xA1, 0x02);
    port_byte_out(0x21, 0x01);
    port_byte_out(0xA1, 0x01);
    port_byte_out(0x21, 0x0);
    port_byte_out(0xA1, 0x0);

    // now we can install irq's from 32 onwards
    set_idt_gate(IRQ0, (uint32_t)irq0);
    set_idt_gate(IRQ1, (uint32_t)irq1);
    set_idt_gate(IRQ2, (uint32_t)irq2);
    set_idt_gate(IRQ3, (uint32_t)irq3);
    set_idt_gate(IRQ4, (uint32_t)irq4);
    set_idt_gate(IRQ5, (uint32_t)irq5);
    set_idt_gate(IRQ6, (uint32_t)irq6);
    set_idt_gate(IRQ7, (uint32_t)irq7);
    set_idt_gate(IRQ8, (uint32_t)irq8);
    set_idt_gate(IRQ9, (uint32_t)irq9);
    set_idt_gate(IRQ10, (uint32_t)irq10);
    set_idt_gate(IRQ11, (uint32_t)irq11);
    set_idt_gate(IRQ12, (uint32_t)irq12);
    set_idt_gate(IRQ13, (uint32_t)irq13);
    set_idt_gate(IRQ14, (uint32_t)irq14);
    set_idt_gate(IRQ15, (uint32_t)irq15);

    set_idt(); // finally load it all (defined in isr.asm)
}

char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isr_handler(registers_data *reg)
{
    /*
    vga_prints("Recieved ISR: ");
    vga_prints(k_inttostr(reg->interrupt_no));
    vga_prints("\n");
    vga_prints(exception_messages[reg->interrupt_no]);
    vga_prints("\n");
    */
    asm("hlt");
}

void register_interrupt_handler(uint8_t n, isr_data handler)
{
    interrupt_handlers[n] = handler;
}

void irq_handler(registers_data *reg)
{
    // send end of interrupt signal to pics
    if (reg->interrupt_no >= 40)
        port_byte_out(0xa0, 0x20);

    port_byte_out(0x20, 0x20);

    if (interrupt_handlers[reg->interrupt_no] != 0)
    {
        isr_data handler = interrupt_handlers[reg->interrupt_no];
        handler(reg);
    }
}