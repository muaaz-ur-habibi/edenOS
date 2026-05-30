#include "../interrupts/isr.h"
#include "../drivers/vga/vga.h"

void init_timer(uint32_t freq);
uint32_t get_tick();