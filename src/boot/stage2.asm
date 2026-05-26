BITS 16       ; BIOS real mode is always 16bits
ORG 0x8000

jmp short start ; These 2 lines are also actually important for this to be recognised as a real FAT filesystem
nop

; ---------------------
; This BIOS Parameter Block (BPB) (and jmp short <blah> nop above) is required to make the output a valid FAT12 floppy disk
; See https://wiki.osdev.org/FAT#BPB_(BIOS_Parameter_Block)
; Values match IBM 3.5" floppy.
;
; Define series of bytes at start of file to make valid BPB (which is part of the spec for FAT)
; We may redefine some of these variables later by reading the actual floppy inserted

OEMLabel          db "ATOSBOOT" ; OEM Identifier (must be 8 bytes aka 8 chars)
BytesPerSector    dw 512        ; Bytes per sector
SectorsPerCluster db 1          ; Sectors per cluster
ReservedForBoot   dw 1          ; Reserved sectors for boot record
NumberOfFats      db 2          ; Number of copies of the FAT
RootDirEntries    dw 224        ; Number of entries in root dir
                                  ; (224 * 32 = 7168 = 14 sectors to read)
LogicalSectors    dw 2880       ; Number of logical sectors
MediaByte         db 0x0F0       ; Media descriptor
SectorsPerFat     dw 9          ; Sectors per FAT
SectorsPerTrack   dw 18         ; Sectors per track (36/cylinder)
Sides             dw 2          ; Number of sides/heads
HiddenSectors     dd 0          ; Number of hidden sectors
LargeSectors      dd 0          ; Number of LBA sectors
DriveNo           dw 0          ; Drive No: 0
Signature         db 0x29       ; Drive signature: 29h for floppy
VolumeID          dd 0x0000     ; Volume ID: any number
VolumeLabel       db "ATOS"     ; Volume Label
FileSystem        db "FAT12"    ; File system type

; ---------------------
; Main bootloader
start:
    cld
    xor ax, ax
    mov ds, ax
    mov ss, ax
    mov sp, 0x8000      ; Set stack pointer below bootloader in memory. Generally safe place.
                        ; as stacks expand down in x86
                        
    call init_message

    cmp dl, 0
      jne disk_error ; DL should contain drive number 0. If it doesn't lets just error
                          ; See http://wiki.osdev.org/System_Initialization_(x86)#BIOS_initialization

    ; Now we need to load the root directory
    mov ax, 19 ; in FAT12 root dir will start at logical sector 19
               ; See https://en.wikipedia.org/wiki/Design_of_the_FAT_file_system
               ; But basically disk looks like [Reserved|FAT tables|Root Dir] so you just need the size of the reserved section + (number of FAT tables * size of sectors per table)
               ; So in our case we have 1 reserved sector of 512, 2 FAT tables (2 for redundancy), and each table takes up 9 sectors
               ; So 1 + 9 * 2 = 19! Therefore our root dir must start at logical sector 19
    call calcRegsFromLogical

    mov si, buffer
    mov bx, ds    ; DS at this point is 8000 aka our offset
    mov es, bx
    mov bx, si    ; Int13h + 02h will write to ES:BX aka segment:offset.
                  ; Result is buffer:8000
    mov ah, 02h   ; Read
    mov al, 14    ; Read 14 sectors aka our whole root dir

    ;pusha
    int 13h
      jc disk_error ; Oops our read failed. On a real system this sort of setup would not be acceptable. We would need to retry.
                     ; As real floppy disks commonly would fail to read the first few times as the disks warmed up.
    ;popa

    ; Root dir is now in buffer
    ; We must search entries for our kernel file :)

    mov si, searching_kernel_msg
    call print_string

    mov ax, ds
    mov es, ax
    mov di, buffer

    mov cx, word [RootDirEntries] ; Move the contents of RootDirEntries (as 2bytes) into cx - Loop below relies on CX
    xor ax, ax

search_root_entries:
    xchg cx, dx ; swap cx and dx so cx value not lost
    mov si, kernel_name ;Input string is our file name
    mov cx, 11 ; counter
    rep cmpsb ; repeat cmpsb 11 times, comparing DI (some value in our buffer) with SI (our kernel name) byte by byte. 11 times cuz that is the length of our kernel filename string
      je found_kernel
    
    add ax, 32 ; Each entry in the root directory table is exactly 32bytes long
    mov di, buffer
    add di, ax
    xchg dx, cx
    loop search_root_entries

    call disk_error; If we reach here the file is not found

    ; The following sections are assuming the kernel does not span more than 1 cluster on the disk. If if it did, we would
    ; Also need to read the FAT tables to find the other associated clusters.

found_kernel:
    mov si, kernel_found_msg
    call print_string

    mov ax, word [es:di+0x0f] ;ES=Buffer, di=11 offset, + 15 = 26 which is the byte that contains the 1st cluster containing our kernel

    add ax, 31 ; The start of our cluster is the cluster number (in AX) + 31 as the boot, fats, and root dir take up the first 31 sectors/clusters)
    call calcRegsFromLogical

    mov ax, 0x1000
    mov es, ax
    mov bx, 0x0000
    xor ax, ax
    mov ah, 02h
    mov al, 15 ; sectors to read

    int 13h ; Read the cluster with the kernel! Yippee
      jc disk_error
    mov dl, byte [bootdevice] ; Make sure the kernel knows the boot device number
    call read_success

    mov si, jumping_into_pm_msg
    call print_string

    cli

    in al, 0x92
    or al, 2
    and al, 0xfe
    out 0x92, al

    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp CODE_SEG:enter_pm

halt:
    jmp halt


; Define segment selectors (offsets into GDT)
CODE_SEG equ gdt_code - gdt_start   ; 0x08
DATA_SEG equ gdt_data - gdt_start   ; 0x10

gdt_start:
; ─────────────────────────────────────────────────────────────────
; Null Descriptor (REQUIRED - CPU checks for this)
; ─────────────────────────────────────────────────────────────────
gdt_null:
    dq 0x0                          ; 8 bytes of zeros
    
; ─────────────────────────────────────────────────────────────────
; Code Segment Descriptor
; Base = 0x00000000, Limit = 0xFFFFF (4GB with granularity)
; Access: Present, Ring 0, Code, Execute/Read
; Flags: 4KB granularity, 32-bit
; ─────────────────────────────────────────────────────────────────
gdt_code:
    dw 0xFFFF               ; Limit (bits 0-15)
    dw 0x0000               ; Base (bits 0-15)
    db 0x00                 ; Base (bits 16-23)
    db 10011010b            ; Access byte:
                            ;   1 = Present
                            ;   00 = Ring 0
                            ;   1 = Code/Data segment
                            ;   1 = Executable (code)
                            ;   0 = Non-conforming
                            ;   1 = Readable
                            ;   0 = Accessed (CPU sets)
    db 11001111b            ; Flags + Limit (bits 16-19):
                            ;   1 = 4KB granularity
                            ;   1 = 32-bit segment
                            ;   0 = Not 64-bit
                            ;   0 = Available
                            ;   1111 = Limit bits 16-19
    db 0x00                 ; Base (bits 24-31)
    
; ─────────────────────────────────────────────────────────────────
; Data Segment Descriptor
; Base = 0x00000000, Limit = 0xFFFFF (4GB with granularity)
; Access: Present, Ring 0, Data, Read/Write
; Flags: 4KB granularity, 32-bit
; ─────────────────────────────────────────────────────────────────
gdt_data:
    dw 0xFFFF               ; Limit (bits 0-15)
    dw 0x0000               ; Base (bits 0-15)
    db 0x00                 ; Base (bits 16-23)
    db 10010010b            ; Access byte:
                            ;   1 = Present
                            ;   00 = Ring 0
                            ;   1 = Code/Data segment
                            ;   0 = Data (not executable)
                            ;   0 = Grows up
                            ;   1 = Writable
                            ;   0 = Accessed
    db 11001111b            ; Flags + Limit (bits 16-19)
    db 0x00                 ; Base (bits 24-31)

gdt_end:

; ─────────────────────────────────────────────────────────────────
; GDT Descriptor (pointer loaded into GDTR)
; ─────────────────────────────────────────────────────────────────
gdt_descriptor:
    dw gdt_end - gdt_start - 1      ; Size (limit = size - 1)
    dd gdt_start                    ; Linear address of GDT

; -----------------------------
; Buffer and signature

%include "stage2_subroutines.asm"

BITS 32
VIDEO_MEMORY equ 0xB8000
SCREEN_W     equ 80
SCREEN_H     equ 25

enter_pm:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esp, 0x90000
    mov ebp, esp

    mov al, 0x0f
    call pm_clear_screen

    mov ebx, pm_msg
    call print_string_pm

    jmp 0x10000

    hlt

pm_halt:
    jmp pm_halt

print_string_pm:
    pusha
    mov edx, VIDEO_MEMORY

print_string_pm_loop:
    mov al, [ebx] ; [ebx] is the address of our character
    mov ah, 0x0f ; white on black text

    cmp al, 0 ; check if end of string
    je print_string_pm_done

    mov [edx], ax ; store character + attribute in video memory
    add ebx, 1 ; next char
    add edx, 2 ; next video memory position

    jmp print_string_pm_loop

print_string_pm_done:
    popa
    ret

pm_clear_screen:
    pusha

    mov edi, VIDEO_MEMORY
    mov ecx, SCREEN_W * SCREEN_H   ; 2000 cells

    mov ah, al                     ; attribute in AH
    mov al, ' '                   ; space character

.clear_loop:
    mov [edi], ax
    add edi, 2
    loop .clear_loop

    popa
    ret

pm_msg db "EdenOS Stage 2 loader in 32 bit mode", 0

times 2048-($-$$) db 0

buffer: ; This label allows us to make sure we don't overwrite our own code in memory