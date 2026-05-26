#include "vga.h"

int vga_row, vga_col, vga_color;
uint16_t *vga_buffer = (uint16_t*) VGA_MEMORY_ADDRESS;
int vga_initialised = 0;

int get_offset(int col, int row) { return 2 * (row * VGA_W + col); }

int get_cursor_offset()
{
    // data 14 high byte, data 15 low byte
    port_byte_out(REGISTER_VGA_CTRL, 14);
	int pos = port_byte_in(REGISTER_VGA_CTRL);
	pos = pos << 8;
	port_byte_out(REGISTER_VGA_CTRL, 15);
	pos += port_byte_in(REGISTER_VGA_DATA);

    return pos * 2;
}

void set_cursor_offset(int offset)
{
    offset/=2;
    port_byte_out(REGISTER_VGA_CTRL, 14);
    port_byte_out(REGISTER_VGA_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REGISTER_VGA_CTRL, 15);
    port_byte_out(REGISTER_VGA_DATA, (unsigned char)(offset & 0xff));
}

void vga_set_color(int fg, int bg)
{
    vga_color = vga_entrycolor(fg, bg);
}

void vga_init()
{
	vga_col = 0, vga_row = 0;
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
	vga_clear();
    vga_initialised = 1;
}

void vga_clear()
{
    if (vga_initialised)
    {vga_col = 0, vga_row = 0;
    set_cursor_offset(get_offset(vga_col, vga_row));

    for (size_t r = 0; r < VGA_H; r++)
	{
		for (size_t c = 0; c < VGA_W; c++)
		{
			size_t idx = r * VGA_W + c;
			vga_buffer[idx] = vga_entrychar(' ', vga_color);
		}
	}}
}

void vga_putchar(char ch)
{
    if (vga_initialised)
	{if (ch == '\n')
	{
		vga_col = 0; vga_row++;
        set_cursor_offset(get_offset(vga_col, vga_row));
        return;
	}
	
	size_t idx = vga_row * VGA_W + vga_col;

	vga_buffer[idx] = vga_entrychar(ch, vga_color);

	vga_col++;

	if (vga_col >= VGA_W)
	{
		vga_col = 0; vga_row++;
	}

    if (vga_row >= VGA_H)
    {
        for (size_t i = 1; i < VGA_H; i++)
        {
            k_memcpy(
                (unsigned char *)(get_offset(0, i-1)+VGA_MEMORY_ADDRESS),
                (unsigned char *)(get_offset(0, i)+VGA_MEMORY_ADDRESS),
                VGA_W*2
            );
        }

        char *last_line = (char *)(get_offset(0, VGA_H) + VGA_MEMORY_ADDRESS);
        for (size_t i = 0; i < VGA_W * 2; i++)
        {
            last_line[i] = 0;
        }
        vga_row = VGA_H - 1;
    }

    set_cursor_offset(get_offset(vga_col, vga_row));}
}

void vga_putchar_at(char ch, int row, int col)
{
    if (vga_initialised)
    {if (ch == '\n')
	{
		vga_col = 0; vga_row++;
        set_cursor_offset(get_offset(vga_col, vga_row));
        return;
	}

    set_cursor_offset(get_offset(col, row));

	size_t idx = row * VGA_W + col;
	vga_buffer[idx] = vga_entrychar(ch, vga_color);

    set_cursor_offset(get_offset(vga_col, vga_row));}
}

void vga_remove_char(int n)
{
    if (vga_initialised)
    {for (size_t i = 0; i < n; i++)
    {
        set_cursor_offset(get_cursor_offset(--vga_col, vga_row));
        vga_putchar(' ');
        set_cursor_offset(get_cursor_offset(--vga_col, vga_row));
    }}
}

void vga_prints(char *str)
{
    if (vga_initialised)
    {for (size_t i = 0; i < k_strlen(str); i++)
    {
        vga_putchar(str[i]);
    }}
}

void vga_prints_at(char *str, int row, int col)
{
    if (vga_initialised)
    {for (size_t i = 0; i < k_strlen(str); i++)
    {
        vga_putchar_at(str[i], row, col+i);
    }}
}