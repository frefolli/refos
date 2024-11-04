#!/bin/bash
BUILDDIR="${1:-.}"
ISO_DIR=${BUILDDIR}/isodir
ISO=${BUILDDIR}/refos.iso
SCRIPTS=$(dirname $0)

rm -rf ${ISO_DIR}
mkdir -p ${ISO_DIR}/boot/grub/
mkdir -p ${ISO_DIR}/EFI/BOOT
mkdir -p ${ISO_DIR}/EFI/BOOT/
cp $SCRIPTS/grub.cfg ${ISO_DIR}/boot/grub/grub.cfg
grub2-mkstandalone -O x86_64-efi -o ${ISO_DIR}/EFI/BOOT/BOOTX64.EFI "boot/grub/grub.cfg=$SCRIPTS/grub-mkstandalone.cfg"
cp ${BUILDDIR}/REFOS.EFI ${ISO_DIR}/EFI/BOOT/REFOS.EFI
grub2-mkrescue -o ${ISO} ${ISO_DIR}
