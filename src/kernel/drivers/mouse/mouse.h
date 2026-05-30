#ifndef KMOUSE
#define KMOUSE

#include "../../interrupts/isr.h"
#include "../vga/vga.h"
#include "../port/port.h"
#include "../../kernel.h"
#include "../../interrupts/isr.h"
#include "../graphics/graphics.h"

void mouse_write(uint8_t write_byte);
void mouse_poll();
void init_mouse();

#endif