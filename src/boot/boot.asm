[ORG 0x7c00]
[BITS 16]

jmp short main
nop

; headers for FAT12 fs
bdb_oem: db 'MSWIN4.1'
bdb_bytes_per_sector: dw 512
bdb_no_sectors_per_cluster: db 1
bdb_reserved_sectors: dw 1
bdb_fat_count: db 2
bdb_dir_entries_count: dw 0e0h
bdb_total_sectors: dw 2880
bdb_media_descriptor_type: db 0f0h
bdb_sectors_per_fat: dw 9
bdb_sectors_per_track: dw 18
bdb_heads: dw 2
bdb_hidden_sectors: dd 0
bdb_large_sector_count: dd 0

ebr_drive_number: db 0
                  db 0
ebr_signature: db 29h
ebr_volume_id: db 12h, 34h, 56h, 78h
ebr_volume_label: db 'EDEN OS    ' ; has to be 11 bytes
ebr_system_id: db 'FAT12   ' ; has to be 8 bytes

main:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00

    mov si, bootloader_boot_msg
    call print

    ; setting values for int 13 call to read stage2 off of sector 2
    mov [ebr_drive_number], dl ; bios passes drive number
    mov ax, 1
    mov cl, 1
    mov bx, 0x8000 ; pointer to buffer on disk

    mov ax, [bdb_sectors_per_fat]
    mov bl, [bdb_fat_count]
    xor bh, bh
    mul bx
    add ax, [bdb_reserved_sectors] ; LBA of root dir
    push ax

    mov ax, [bdb_dir_entries_count]
    shl ax, 5
    xor dx, dx

    div word [bdb_bytes_per_sector]

    test dx, dx
    jz root_dir
    inc ax

root_dir:
    mov cl, al
    pop ax
    mov dl, [ebr_drive_number]
    mov bx, buffer

    call disk_read

    xor bx, bx
    mov di, buffer

search_stage2:
    mov si, stage2_file
    mov cx, 11
    push di
    repe cmpsb
    pop di
    je found_stage2

    add di, 32
    inc bx
    cmp bx, [bdb_dir_entries_count]
    jl search_stage2

    jmp stage2_not_found

stage2_not_found:
    mov si, stage2_nf_msg
    call print

    jmp halt

found_stage2: ; loads the fat to memory
    mov ax, [di+26]
    mov [stage2_cluster], ax

    mov ax, [bdb_reserved_sectors]
    mov bx, buffer
    mov cl, [bdb_sectors_per_fat]
    mov dl, [ebr_drive_number]

    call disk_read

    mov bx, stage2_load_seg
    mov es, bx
    mov bx, stage2_load_off

stage2_load_loop: ; attempts to find the stage2 file that exists
    mov ax, [stage2_cluster]
    add ax, 31 ; change to support other disks
    mov cl, 1
    mov dl, [ebr_drive_number]

    call disk_read
    
    add bx, [bdb_bytes_per_sector]
    mov ax, [stage2_cluster]
    mov cx, 3
    mul cx
    mov cx, 2
    div cx

    mov si, buffer
    add si, ax
    mov ax, [ds:si]

    or dx, dx
    jz even

odd:
    shr ax, 4
    jmp next_cluster_after

even:
    and ax, 0x0fff
    jmp next_cluster_after

next_cluster_after:
    cmp ax, 0xff8
    jae read_finish

    mov [stage2_cluster], ax

    jmp stage2_load_loop

read_finish:
    mov si, stage2_f_msg
    call print
    
    mov dl, [ebr_drive_number]
    mov ax, stage2_load_seg
    mov ds, ax
    mov es, ax

    jmp stage2_load_seg:stage2_load_off

    jmp halt

halt:
    jmp halt

lba_to_chs:
    push ax
    push dx

    xor dx, dx
    div word [bdb_sectors_per_track]

    inc dx

    mov cx, dx
    xor dx, dx

    div word [bdb_heads]
    mov dh, dl

    mov ch, al
    shl ah, 6
    or cl, ah
    
    pop ax
    mov dl, al
    pop ax

    ret

disk_read:
    ; convert LBA to CHS
    push ax
    push bx
    push cx
    push dx
    push di

    call lba_to_chs

    mov ah, 02h
    mov di, 3

retry_read:
    stc ; sets carry since we are looping atleast di times, and some bios dont set carry? idk
    int 13h
    jnc done_read

    call disk_reset ; resets disk driver if error occurs
    dec di
    test di, di
    jnz retry_read

disk_reset:
    pusha
    mov ah, 0
    ; drive number already set from main read_disk func
    stc
    int 13h
    jc failed_disk_read

    popa

done_read:
    pop di
    pop dx
    pop cx
    pop bx
    pop ax

    ret

failed_disk_read:
    mov si, disk_read_fail_msg
    call print
    jmp halt    

print:
    push si
    push ax
    push bx

print_loop:
    lodsb
    or al, al
    jz done_print

    mov ah, 0x0e
    mov bh, 0
    int 0x10

    jmp print_loop

done_print:
    pop bx
    pop ax
    pop si

    ret

bootloader_boot_msg:
    db 'EdenOS boot successful', 0x0d, 0x0a, 0
disk_read_fail_msg:
    db 'Disk read failed', 0x0d, 0x0a, 0
stage2_f_msg:
    db 'stage2.bin is found', 0x0d, 0x0a, 0
stage2_nf_msg:
    db 'stage2.bin wasnt found', 0x0d, 0x0a, 0

stage2_file: db 'STAGE2  BIN'
stage2_cluster dw 0

stage2_load_seg equ 0
stage2_load_off equ 0x8000

times 510-($-$$) db 0
dw 0aa55h

buffer: