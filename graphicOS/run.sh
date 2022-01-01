#assemble boot.s file
as --32 boot/boot.s -o compiled/boot.o

clear

#compile kernel.c file
if (gcc -m32 -c kernel.c -o compiled/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra) ; then

#linking the kernel with kernel.o and boot.o files
ld -m elf_i386 -T boot/linker.ld compiled/kernel.o compiled/boot.o -o compiled/MyOS.bin -nostdlib

#check MyOS.bin file is x86 multiboot file or not
grub-file --is-x86-multiboot compiled/MyOS.bin

#building the iso file
mkdir -p isodir/boot/grub
cp compiled/MyOS.bin isodir/boot/MyOS.bin
cp boot/grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o compiled/MyOS.iso isodir

#run it in qemu
qemu-system-x86_64 -drive file=compiled/MyOS.iso,format=raw

fi