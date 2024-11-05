BUILDDIR=./builddir

ISO_DIR=${BUILDDIR}/iso_dir
ISO=${BUILDDIR}/refos.iso
REFOS_EFI=${BUILDDIR}/REFOS.EFI
REFOS_SO=${BUILDDIR}/refos.so

GNU_EFI_CRTO=/usr/lib/crt0-efi-x86_64.o
GNU_EFI_LDS=/usr/lib/elf_x86_64_efi.lds
LIB_EFI=/usr/lib/libefi.a
LIB_GNUEFI=/usr/lib/libgnuefi.a

CFLAGS=-fpic -ffreestanding -fno-stack-protector -fno-stack-check -fshort-wchar -mno-red-zone -maccumulate-outgoing-args
LDFLAGS=-shared -Bsymbolic -T${GNU_EFI_LDS} ${GNU_EFI_CRTO}
OBJFLAGS=-j .text -j .sdata -j .data -j .rodata -j .dynamic -j .dynsym  -j .rel -j .rela -j .rel.* -j .rela.* -j .reloc --target efi-app-x86_64 --subsystem=10

C_SOURCES=$(wildcard src/*.c)
C_OBJECTS=$(patsubst src/%, $(BUILDDIR)/%, $(patsubst %.c, %.o, ${C_SOURCES}))

@all: ${BUILDDIR} ${ISO}

${BUILDDIR}:
	mkdir -p ${BUILDDIR}

$(C_OBJECTS): ${BUILDDIR}/%.o: src/%.c
	gcc ${CFLAGS} -c $^ -o $@

${REFOS_SO}: ${C_OBJECTS}
	ld ${LDFLAGS} $^ -o $@ ${LIB_EFI} ${LIB_GNUEFI}

${REFOS_EFI}: ${REFOS_SO}
	objcopy ${OBJFLAGS} $^ $@

${ISO}: ${REFOS_EFI}
	rm -rf ${ISO_DIR}
	mkdir -p ${ISO_DIR}/boot/grub/
	mkdir -p ${ISO_DIR}/EFI/BOOT
	mkdir -p ${ISO_DIR}/EFI/BOOT/
	cp ./scripts/grub.cfg ${ISO_DIR}/boot/grub/grub.cfg
	grub2-mkstandalone -O x86_64-efi -o ${ISO_DIR}/EFI/BOOT/BOOTX64.EFI "boot/grub/grub.cfg=./scripts/grub-mkstandalone.cfg"
	cp ${BUILDDIR}/REFOS.EFI ${ISO_DIR}/EFI/BOOT/REFOS.EFI
	grub2-mkrescue -o ${ISO} ${ISO_DIR}

run: ${ISO}
	qemu-system-x86_64 -machine q35 -m 256 -smp 2 -net none \
		-global driver=cfi.pflash01,property=secure,value=on \
		-drive if=pflash,format=raw,unit=0,file=/usr/share/edk2/ovmf/OVMF_CODE.fd,readonly=on \
		-drive if=ide,format=raw,media=cdrom,file=${ISO}
		#-drive if=pflash,format=raw,unit=1,file=/usr/share/edk2/ovmf/OVMF_VARS.fd \

clean:
	rm -rf ${BUILDDIR}
