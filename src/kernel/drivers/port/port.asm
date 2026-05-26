global port_byte_in
global port_byte_out

port_byte_in:
    mov dx, [esp + 4]
    in al, dx
    ret

port_byte_out:
    mov dx, [esp + 4]   ; port (1st argument)
    mov al, [esp + 8]   ; data (2nd argument)
    out dx, al          ; write AL to port DX
    ret