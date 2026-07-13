global port_byte_in
global port_byte_out
global port_long_in
global port_long_out
global check_mtrr_bit

port_byte_in:
    mov dx, [esp + 4]
    in al, dx
    ret

port_byte_out:
    mov dx, [esp + 4]   ; port (1st argument)
    mov al, [esp + 8]   ; data (2nd argument)
    out dx, al          ; write AL to port DX
    ret

port_long_in:
    mov dx, [esp + 4]
    in eax, dx
    ret

port_long_out:
    mov dx, [esp + 4]
    mov eax, [esp + 8]
    out dx, eax
    ret

check_mtrr_bit:
    mov eax, 1
    cpuid
    shr edx, 12
    and edx, 1
    mov eax, edx
    ret