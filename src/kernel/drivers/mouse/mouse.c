#include "mouse.h"

uint8_t mouse_cycle = 0;
uint8_t mouse_bytes[3];
int mouse_x, mouse_y, mouseDown = 0;

void mouse_write(uint8_t write_byte)
{
    while ((port_byte_in(0x64) & 0x02));
    port_byte_out(0x64, 0xD4);
    while ((port_byte_in(0x64) & 0x02));
    port_byte_out(0x60, write_byte);
    while (!(port_byte_in(0x64) & 0x01));
    port_byte_in(0x60); 
}

void mouse_poll(int *mousex, int *mousey)
{
    uint8_t stat = port_byte_in(0x64);
        
        if ((stat & 0x01))
        {
            if (stat & 0x20)
            {
                while (port_byte_in(0x64) & 0x01)
                {
                    uint8_t data = port_byte_in(0x60);
                    if (mouse_cycle == 0) {
                        if (!(data & 0x08)) continue; 
                        mouse_bytes[0] = data;
                        mouse_cycle = 1;
                    } 
                    else if (mouse_cycle == 1) {
                        mouse_bytes[1] = data;
                        mouse_cycle = 2;
                    } 
                    else if (mouse_cycle == 2) {
                        mouse_bytes[2] = data;
                        mouse_cycle = 0; 

                        uint8_t flags = mouse_bytes[0];
                        int16_t rel_x = mouse_bytes[1];
                        int16_t rel_y = mouse_bytes[2];

                        if (flags & 0x10) rel_x |= 0xFF00; 
                        if (flags & 0x20) rel_y |= 0xFF00;

                        int16_t abs_x = (rel_x < 0) ? -rel_x : rel_x;
                        int16_t abs_y = (rel_y < 0) ? -rel_y : rel_y;
                        if (abs_x > 5) rel_x *= 2;
                        if (abs_y > 5) rel_y *= 2;

                        mouse_x += rel_x;
                        mouse_y -= rel_y;

                        // 0 = L, 1 = R, 2 = M
                        mouseDown = (flags & 0x01); 

                        if (mouse_x < 0) mouse_x = 0;
                        if (mouse_y < 0) mouse_y = 0;
                        if (mouse_x >= 1024)  mouse_x = 1024 - 1;
                        if (mouse_y >= 768) mouse_y = 768 - 1;

                        *mousex = mouse_x;
                        *mousey = mouse_y;
                    }
                }
            }
        }
}

void init_mouse()
{
    while ((port_byte_in(0x64) & 0x02));
    port_byte_out(0x64, 0xA8); 

    mouse_write(0xF6);
    mouse_write(0xF3);
    mouse_write(200);
    mouse_write(0xF4);
}