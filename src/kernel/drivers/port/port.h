#ifndef KPORT
#define KPORT

#include <stdint.h>

extern uint8_t port_byte_in(uint16_t port);
extern void port_byte_out(uint16_t port, uint8_t data);
extern uint32_t port_long_in(uint16_t port);
extern void port_long_out(uint16_t port, uint32_t data);
extern uint32_t check_mtrr_bit();

#endif