cd src/kernel
./build.sh
cd ../../

echo "Copying kernel.elf"
cp src/kernel/build/kernel.elf iso/boot/

echo "Building iso"
grub-mkrescue -o iso/run/os.iso iso/
echo "ISO built"

echo "Running"
powershell.exe -Command "qemu-system-i386 -cdrom $(wslpath -w iso/run/os.iso) -display sdl"