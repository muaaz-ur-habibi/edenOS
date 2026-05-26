echo "Building Kernal ASM Files"
nasm -f elf32 kernel.asm -o kasm.o
nasm -f elf32 drivers/port/port.asm -o kport.o
nasm -f elf32 interrupts/isr.asm -o kisrasm.o

CFLAGS="-m32 -ffreestanding -fno-pic -fno-pie -nostdlib -nostartfiles -nodefaultlibs -fno-stack-protector -mno-red-zone"
echo "Building Kernel C Files"
gcc $CFLAGS -c kernel.c -o kc.o
gcc $CFLAGS -c drivers/vga/vga.c -o kvga.o
gcc $CFLAGS -c interrupts/isr.c -o kisr.o
gcc $CFLAGS -c interrupts/idt.c -o kidt.o
gcc $CFLAGS -c irqs/timer.c -o kirqtimer.o
gcc $CFLAGS -c drivers/keyboard/keyboard.c -o kkb.o
gcc $CFLAGS -c drivers/mouse/mouse.c -o kmouse.o
gcc $CFLAGS -c utils/memory.c -o kutilsmem.o
gcc $CFLAGS -c utils/string.c -o kutilstr.o
gcc $CFLAGS -c utils/ui.c -o kui.o
gcc $CFLAGS -c drivers/graphics/graphics.c -o kgraphics.o

echo "Linking kernel files..."
# note: not linking kmouse.o at the moment (keeps crashing kernel)
ld -m elf_i386 -T link.ld -o kernel.elf kasm.o kc.o kvga.o kport.o kisrasm.o kisr.o kidt.o kirqtimer.o kkb.o kutilsmem.o kutilstr.o kgraphics.o kui.o

echo "Finished building kernel.elf"

#echo "Running..."
# cant use this command with multiboot2 (grub version)
#powershell.exe -Command "qemu-system-i386 -kernel kernel.elf"