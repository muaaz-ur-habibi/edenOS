#ifndef KKB
#define KKB

#define MAX_INPUT_CHARS 512

#include "../../interrupts/isr.h"
#include "../vga/vga.h"
#include "../port/port.h"
#include "../../kernel.h"

void init_keyboard();

#endif