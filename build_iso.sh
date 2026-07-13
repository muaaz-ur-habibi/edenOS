cd src/kernel
./build.sh
cd ../../

echo "Copying kernel.elf"
cp src/kernel/build/kernel.elf iso/boot/

echo "Building iso"
grub-mkrescue -o iso/run/edenos.iso iso/
echo "ISO built"

echo "Running"
powershell.exe -Command "qemu-system-x86_64 -cdrom $(wslpath -w iso/run/edenos.iso) -display sdl"