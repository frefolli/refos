#!/bin/bash
set -e

OUT_IMG=./builddir/fat.iso

GNU_EFI_CRTO=/usr/lib/crt0-efi-x86_64.o
GNU_EFI_LDS=/usr/lib/elf_x86_64_efi.lds
CFLAGS='-fpic -ffreestanding -fno-stack-protector -fno-stack-check -fshort-wchar -mno-red-zone -maccumulate-outgoing-args'
LDFLAGS="-shared -Bsymbolic -T$GNU_EFI_LDS $GNU_EFI_CRTO"
OBJFLAGS='-j .text -j .sdata -j .data -j .rodata -j .dynamic -j .dynsym  -j .rel -j .rela -j .rel.* -j .rela.* -j .reloc --target efi-app-x86_64 --subsystem=10'

function compile-with-gcc() {
  gcc $CFLAGS -c src/main.c -o builddir/main.o
  ld $LDFLAGS ./builddir/main.o -o ./builddir/main.so /usr/lib/libefi.a /usr/lib/libgnuefi.a
  objcopy $OBJFLAGS ./builddir/main.so ./builddir/BOOTX64.EFI
}

function create-fat() {
  dd if=/dev/zero of=$OUT_IMG bs=1k count=1440
  mformat -i $OUT_IMG -f 1440 ::
  mmd -i $OUT_IMG ::/EFI
  mmd -i $OUT_IMG ::/EFI/BOOT
  mcopy -i $OUT_IMG ./builddir/BOOTX64.EFI ::/EFI/BOOT
}

rm -rf ./builddir
mkdir -p ./builddir
compile-with-gcc
create-fat
