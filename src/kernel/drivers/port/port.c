#include "port.h"

// we are using nasm 'port.asm' implementation for now, not this

unsigned char port_byte_in(unsigned short port)
{
    unsigned char res;

    __asm__("in %%dx, %%al" : "=a" (res) : "d" (port));

    return res;
}

void port_byte_out(unsigned short port, unsigned char data)
{
    __asm__("out %%dx, %%al" :: "a" (data), "d" (port));
}