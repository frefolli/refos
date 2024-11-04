#!/bin/bash
set -e

ISO_DIR=./builddir/iso_dir
ISO=./builddir/refos.iso

GNU_EFI_CRTO=/usr/lib/crt0-efi-x86_64.o
GNU_EFI_LDS=/usr/lib/elf_x86_64_efi.lds
CFLAGS='-fpic -ffreestanding -fno-stack-protector -fno-stack-check -fshort-wchar -mno-red-zone -maccumulate-outgoing-args'
LDFLAGS="-shared -Bsymbolic -T$GNU_EFI_LDS $GNU_EFI_CRTO"
OBJFLAGS='-j .text -j .sdata -j .data -j .rodata -j .dynamic -j .dynsym  -j .rel -j .rela -j .rel.* -j .rela.* -j .reloc --target efi-app-x86_64 --subsystem=10'

function compile-with-gcc() {
  gcc $CFLAGS -c src/main.c -o builddir/main.o
  ld $LDFLAGS ./builddir/main.o -o ./builddir/main.so /usr/lib/libefi.a /usr/lib/libgnuefi.a
  objcopy $OBJFLAGS ./builddir/main.so ./builddir/REFOS.EFI
}

function create-iso() {
  mkdir -p $ISO_DIR/boot/grub/
  cp ./scripts/grub.cfg $ISO_DIR/boot/grub/grub.cfg
  mkdir -p $ISO_DIR/EFI/BOOT
  grub2-mkstandalone -O x86_64-efi -o $ISO_DIR/EFI/BOOT/BOOTX64.EFI "boot/grub/grub.cfg=./scripts/grub-mkstandalone.cfg"
  mkdir -p $ISO_DIR/EFI/BOOT/
  cp ./builddir/REFOS.EFI $ISO_DIR/EFI/BOOT/REFOS.EFI
  grub2-mkrescue -o $ISO $ISO_DIR
}

rm -rf ./builddir
mkdir -p ./builddir
compile-with-gcc
create-iso
