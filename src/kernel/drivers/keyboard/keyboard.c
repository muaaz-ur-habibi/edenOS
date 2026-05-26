#include "keyboard.h"

static char input_chars[MAX_INPUT_CHARS];
int current_idx;

char *get_letter_from_scancode(uint8_t scancode) {
    switch (scancode) {
        case 0x0:
            return "ERROR";
            break;
        case 0x1:
            return "ESC";
            break;
        case 0x2:
            return "1";
            break;
        case 0x3:
            return "2";
            break;
        case 0x4:
            return "3";
            break;
        case 0x5:
            return "4";
            break;
        case 0x6:
            return "5";
            break;
        case 0x7:
            return "6";
            break;
        case 0x8:
            return "7";
            break;
        case 0x9:
            return "8";
            break;
        case 0x0A:
            return "9";
            break;
        case 0x0B:
            return "0";
            break;
        case 0x0C:
            return "-";
            break;
        case 0x0D:
            return "+";
            break;
        case 0x0E:
            return "BACKSPACE";
            break;
        case 0x0F:
            return "Tab";
            break;
        case 0x10:
            return "q";
            break;
        case 0x11:
            return "w";
            break;
        case 0x12:
            return "e";
            break;
        case 0x13:
            return "r";
            break;
        case 0x14:
            return "t";
            break;
        case 0x15:
            return "y";
            break;
        case 0x16:
            return "u";
            break;
        case 0x17:
            return "i";
            break;
        case 0x18:
            return "o";
            break;
        case 0x19:
            return "p";
            break;
		case 0x1A:
			return "[";
			break;
		case 0x1B:
			return "]";
			break;
		case 0x1C:
			return "ENTER";
			break;
		case 0x1D:
			return "LCTRL";
			break;
		case 0x1E:
			return "a";
			break;
		case 0x1F:
			return "s";
			break;
        case 0x20:
            return "d";
            break;
        case 0x21:
            return "f";
            break;
        case 0x22:
            return "g";
            break;
        case 0x23:
            return "h";
            break;
        case 0x24:
            return "j";
            break;
        case 0x25:
            return "k";
            break;
        case 0x26:
            return "l";
            break;
        case 0x27:
            return ";";
            break;
        case 0x28:
            return "'";
            break;
        case 0x29:
            return "`";
            break;
		case 0x2A:
			return "LSHIFT";
			break;
		case 0x2B:
			return "\\";
			break;
		case 0x2C:
			return "z";
			break;
		case 0x2D:
			return "x";
			break;
		case 0x2E:
			return "c";
			break;
		case 0x2F:
			return "v";
			break;
        case 0x30:
            return "b";
            break;
        case 0x31:
            return "n";
            break;
        case 0x32:
            return "m";
            break;
        case 0x33:
            return ",";
            break;
        case 0x34:
            return ".";
            break;
        case 0x35:
            return "/";
            break;
        case 0x36:
            return "RSHIFT";
            break;
        case 0x37:
            return "KEYPAD*";
            break;
        case 0x38:
            return "LALT";
            break;
        case 0x39:
            return " ";
            break;
        default:
            /* 'keyup' event corresponds to the 'keydown' + 0x80 
             * it may still be a scancode we haven't implemented yet, or
             * maybe a control/escape sequence */
            if (scancode <= 0x7f) {
                return NULL;
            } else if (scancode <= 0x39 + 0x80) {
                /* this handles the keyup part, but we dont need that functionality for now
                return get_letter_from_scancode(scancode - 0x80);
                */
                return NULL;
            } else { return NULL; }
            break;
    }
}

void keyboard_append_char(char ch)
{
    input_chars[current_idx++] = ch;
}

char keyboard_pop_char()
{
    return input_chars[current_idx--];
}

void keyboard_clear_char_buffer()
{
    for (size_t i = 0; i < MAX_INPUT_CHARS; i++)
    {
        input_chars[i] = 0;
    }
    current_idx = 0;
}

static void keyboard_callback(registers_data *reg)
{
    // pic gives scancode in port 0x60
    uint8_t sc = port_byte_in(0x60);
    char *sc_ascii = get_letter_from_scancode(sc);

    if (k_strlen(sc_ascii) == 1 && sc_ascii != NULL)
    {
        keyboard_append_char(sc_ascii[0]);
        vga_prints(sc_ascii);
    } else
    {
        if (k_strcmp(sc_ascii, "ENTER"))
        {
            keyboard_append_char('\0');
            int n_inputs = 0;
            char **inputs = k_strsplt(input_chars, ' ', &n_inputs);
            handle_user_inp(inputs, n_inputs);
            keyboard_clear_char_buffer();
            vga_prints("\n>> ");
        } else
        if (k_strcmp(sc_ascii, "BACKSPACE"))
        {
            keyboard_pop_char();
            vga_remove_char(1);
        }
    }
}

void init_keyboard()
{
    register_interrupt_handler(IRQ1, keyboard_callback);
}