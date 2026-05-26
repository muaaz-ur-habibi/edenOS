#ifndef KPORT
#define KPORT

#include <stdint.h>

unsigned char port_byte_in(unsigned short port);
void port_byte_out(unsigned short port, unsigned char data);

extern uint8_t asm_port_byte_in(uint16_t port);
extern uint8_t asm_port_byte_out(uint16_t port, uint16_t data);

#endif