#ifndef KMOUSE
#define KMOUSE

#include "../../interrupts/isr.h"
#include "../vga/vga.h"
#include "../port/port.h"
#include "../../kernel.h"
#include "../../interrupts/isr.h"
#include "../graphics/graphics.h"

static inline void mouse_wait(unsigned char a_type);
static inline void mouse_write(unsigned char a_write);
unsigned char mouse_read();
void mouse_install();

#endif