nasm bootloader.asm -f bin -o boot.bin
nasm stage2.asm -f bin -o stage2.bin

#cat boot.bin stage2.bin > boot.img
#powershell.exe -Command "qemu-system-i386 -fda "$(wslpath -w boot.img)""