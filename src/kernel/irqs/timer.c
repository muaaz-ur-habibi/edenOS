#include "timer.h"

uint32_t tick = 0;

static void timer_callback(registers_data *reg)
{
    tick++;
}

uint32_t get_tick() { return tick; }

void init_timer(uint32_t freq)
{
    //vga_prints("Initializing timer\n");
    register_interrupt_handler(IRQ0, timer_callback);

    uint32_t divisor = 1193180 / freq;
    uint8_t low = (uint8_t)(divisor & 0xff);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xff);

    port_byte_out(0x43, 0x36);
    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
}