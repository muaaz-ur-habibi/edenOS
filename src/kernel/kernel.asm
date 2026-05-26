bits 32

section .multiboot ; multiboot headers
; multiboot 2 header
align 8
header_start:
dd 0xe85250d6
dd 0
dd header_end - header_start
dd -(0xe85250d6 + 0 + (header_end - header_start))

align 8
dw 5
dw 0
dd 20
dd 1024
dd 768
dd 32

align 8
dw 0
dw 0
dd 20
header_end:

; multiboot 1 header
;align 4
;dd 0x1badb002
;dd 0x00
;dd - (0x1badb002 + 0x00)

section .text
global start
extern k_main

start:
    mov esp, 0x8000

    lgdt [gdt_descriptor]


    mov ecx, eax

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:flush_cs

flush_cs:
    cli

    ; pass parameters to k_main
    push ebx ; multiboot info
    push ecx ; MAGIC
    call k_main
    
halt:
    cli
    hlt
    jmp halt

section .data
gdt_start:

gdt_null: dq 0x0000000000000000
gdt_code: dq 0x00CF9A000000FFFF
gdt_data: dq 0x00CF92000000FFFF

gdt_end:

gdt_descriptor:
dw gdt_end - gdt_start - 1
dd gdt_start