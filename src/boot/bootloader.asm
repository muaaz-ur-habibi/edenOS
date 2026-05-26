    BITS 16       ; BIOS real mode is always 16bits
    ORG 0x7C00

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
    mov sp, 0x7C00      ; Set stack pointer below bootloader in memory. Generally safe place.
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
    mov bx, ds    ; DS at this point is 7C00 aka our offset
    mov es, bx
    mov bx, si    ; Int13h + 02h will write to ES:BX aka segment:offset.
                  ; Result is buffer:7C00
    mov ah, 02h   ; Read
    mov al, 14    ; Read 14 sectors aka our whole root dir

    ;pusha
    int 13h
      jc disk_error ; Oops our read failed. On a real system this sort of setup would not be acceptable. We would need to retry.
                     ; As real floppy disks commonly would fail to read the first few times as the disks warmed up.
    ;popa

    ; Root dir is now in buffer
    ; We must search entries for our stage2 file :)
    mov ax, ds
    mov es, ax
    mov di, buffer

    mov cx, word [RootDirEntries] ; Move the contents of RootDirEntries (as 2bytes) into cx - Loop below relies on CX
    xor ax, ax

search_root_entries:
    xchg cx, dx ; swap cx and dx so cx value not lost
    mov si, stage2_name ;Input string is our file name
    mov cx, 11 ; counter
    rep cmpsb ; repeat cmpsb 11 times, comparing DI (some value in our buffer) with SI (our stage2 name) byte by byte. 11 times cuz that is the length of our stage2 filename string
      je found_stage2
    
    add ax, 32 ; Each entry in the root directory table is exactly 32bytes long
    mov di, buffer
    add di, ax
    xchg dx, cx
    loop search_root_entries

    call disk_error; If we reach here the file is not found

    ; The following sections are assuming the stage2 does not span more than 1 cluster on the disk. If if it did, we would
    ; Also need to read the FAT tables to find the other associated clusters.

found_stage2:
    mov ax, word [es:di+0x0f] ;ES=Buffer, di=11 offset, + 15 = 26 which is the byte that contains the 1st cluster containing our stage2

    add ax, 31 ; The start of our cluster is the cluster number (in AX) + 31 as the boot, fats, and root dir take up the first 31 sectors/clusters)
    call calcRegsFromLogical

    mov ax, 0x0000
    mov es, ax
    mov bx, 0x8000
    xor ax, ax
    mov ah, 02h
    mov al, 4

    int 13h ; Read the cluster with the stage2! Yippee
      jc disk_error
    mov dl, byte [bootdevice] ; Make sure the stage2 knows the boot device number
    call read_success
    jmp 0x0000:0x8000 ; JUMP TO stage2! :D
    

; -----------------------------
; Buffer and signature

%include "bootloader_subroutines.asm"

times 510-($-$$) db 0
dw 0xAA55       ; SIGNATURE

buffer: ; This label allows us to make sure we don't overwrite our own code in memory
        ; by placing things after this (stack is under this code at 7C00)
        ; this buffer must start then at 7C00 + 512B