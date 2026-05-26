[extern isr_handler]
[extern irq_handler]

common_isr:
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp ; pushing the reg struct pointer
    call isr_handler ; calling C handler
    pop eax

    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 8 ;  cleans pushed error code and isr no

    sti
    iret

; similiar to common isr except for handler
common_irq:
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp
    call irq_handler
    pop eax

    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 8

    sti
    iret

; isr handlers
global isr_div0
global isr_dbg
global isr_non_mskble
global isr_int3
global isr_into
global isr_oob
global isr_opc
global isr_coproc
global isr_2fault
global isr_coproc_segoverrun
global isr_bad_tss
global isr_seg_unav
global isr_stack_fault
global isr_gen_protec_fault
global isr_page_fault
global isr_rsrv_e
global isr_float
global isr_align_chck
global isr_mach_chck
global isr_rsrv1
global isr_rsrv2
global isr_rsrv3
global isr_rsrv4
global isr_rsrv5
global isr_rsrv6
global isr_rsrv7
global isr_rsrv8
global isr_rsrv9
global isr_rsrv10
global isr_rsrv11
global isr_rsrv12
global isr_rsrv13
; irq handlers
global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

isr_div0:
    cli
    push byte 0
    push byte 0
    jmp common_isr

isr_dbg:
    cli
    push byte 0
    push byte 1
    jmp common_isr

isr_non_mskble:
    cli
    push byte 0
    push byte 2
    jmp common_isr

isr_int3:
    cli
    push byte 0
    push byte 3
    jmp common_isr

isr_into:
    cli
    push byte 0
    push byte 4
    jmp common_isr

isr_oob:
    cli
    push byte 0
    push byte 5
    jmp common_isr

isr_opc:
    cli
    push byte 0
    push byte 6
    jmp common_isr

isr_coproc:
    cli
    push byte 0
    push byte 7
    jmp common_isr

isr_2fault:
    cli
    push byte 8
    jmp common_isr

isr_coproc_segoverrun:
    cli
    push byte 0
    push byte 9
    jmp common_isr
    
isr_bad_tss:
    cli
    push byte 10
    jmp common_isr
    
isr_seg_unav:
    cli
    push byte 11
    jmp common_isr
    
isr_stack_fault:
    cli
    push byte 12
    jmp common_isr
    
isr_gen_protec_fault:
    cli
    push byte 13
    jmp common_isr
    
isr_page_fault:
    cli
    push byte 14
    jmp common_isr
    
isr_rsrv_e:
    cli
    push byte 0
    push byte 15
    jmp common_isr
    
isr_float:
    cli
    push byte 0
    push byte 16
    jmp common_isr
    
isr_align_chck:
    cli
    push byte 17
    jmp common_isr
    
isr_mach_chck:
    cli
    push byte 0
    push byte 18
    jmp common_isr
    
isr_rsrv1:
    cli
    push byte 0
    push byte 20
    jmp common_isr
    
isr_rsrv2:
    cli
    push byte 0
    push byte 21
    jmp common_isr
    
isr_rsrv3:
    cli
    push byte 0
    push byte 22
    jmp common_isr
    
isr_rsrv4:
    cli
    push byte 0
    push byte 23
    jmp common_isr
    
isr_rsrv5:
    cli
    push byte 0
    push byte 24
    jmp common_isr
    
isr_rsrv6:
    cli
    push byte 0
    push byte 25
    jmp common_isr
    
isr_rsrv7:
    cli
    push byte 0
    push byte 26
    jmp common_isr
    
isr_rsrv8:
    cli
    push byte 0
    push byte 27
    jmp common_isr
    
isr_rsrv9:
    cli
    push byte 0
    push byte 28
    jmp common_isr
    
isr_rsrv10:
    cli
    push byte 0
    push byte 29
    jmp common_isr

isr_rsrv11:
    cli
    push byte 0
    push byte 19
    jmp common_isr
    
isr_rsrv12:
    cli
    push byte 0
    push byte 30
    jmp common_isr
    
isr_rsrv13:
    cli
    push byte 0
    push byte 31
    jmp common_isr

irq0:
    cli
    push byte 0
    push byte 32
    jmp common_irq
irq1:
    cli
    push byte 0
    push byte 33
    jmp common_irq
irq2:
    cli
    push byte 0
    push byte 34
    jmp common_irq
irq3:
    cli
    push byte 0
    push byte 35
    jmp common_irq
irq4:
    cli
    push byte 0
    push byte 36
    jmp common_irq
irq5:
    cli
    push byte 0
    push byte 37
    jmp common_irq
irq6:
    cli
    push byte 0
    push byte 38
    jmp common_irq
irq7:
    cli
    push byte 0
    push byte 39
    jmp common_irq
irq8:
    cli
    push byte 0
    push byte 40
    jmp common_irq
irq9:
    cli
    push byte 0
    push byte 41
    jmp common_irq
irq10:
    cli
    push byte 0
    push byte 42
    jmp common_irq
irq11:
    cli
    push byte 0
    push byte 43
    jmp common_irq
irq12:
    cli
    push byte 0
    push byte 44
    jmp common_irq
irq13:
    cli
    push byte 0
    push byte 45
    jmp common_irq
irq14:
    cli
    push byte 0
    push byte 46
    jmp common_irq
irq15:
    cli
    push byte 0
    push byte 47
    jmp common_irq