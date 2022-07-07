ISO=os.iso
ISO_DIR=iso
QEMU_OPTION=-d cpu_reset -D ../log/log.txt -boot d -cdrom $(ISO) -m 512
TARGET=BIOS

@all:
	echo -e ""

compile: config compile_$(TARGET)

compile_BIOS: $(ISO_DIR)/boot/kernel.bin $(ISO)

compile_UEFI: $(ISO_DIR)/boot/EFI/BOOTX64.EFI $(ISO_DIR)/boot/kernel.bin $(ISO)

$(ISO): $(ISO_DIR)/boot/kernel.bin
	grub-mkrescue -o $(ISO) $(ISO_DIR)

$(ISO_DIR)/boot/kernel.bin: *.o
	ld -T link.ld --nmagic $^ -o $@

$(ISO_DIR)/boot/EFI/BOOTX64.EFI:
	mkdir -p $(ISO_DIR)/boot/EFI
	grub-mkstandalone -O x86_64-efi -o $(ISO_DIR)/boot/EFI/BOOTX64.EFI "boot/grub/grub.cfg=build/grub.cfg"

run: compile run_$(TARGET)

run_BIOS:
	qemu-system-x86_64 $(QEMU_OPTION)

run_UEFI:
	qemu-system-x86_64 $(QEMU_OPTION) -bios /usr/share/ovmf/x64/OVMF.fd

clean:
	rm -rf *.o *.bin $(ISO_DIR) $(ISO)

config:
	cp -r ../skel/$(ISO_DIR)_$(TARGET) $(ISO_DIR)
	cp ../skel/grub.cfg grub.cfg
	cp ../skel/link.ld link.ld

bochs:
	bochs -f bochsrc.txt -q
