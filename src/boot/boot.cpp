#include "boot/boot.hpp"
#include "boot/multiboot2.hpp"

bool checkMagic(uint64_t magic) {
    return (magic == MULTIBOOT2_BOOTLOADER_MAGIC);
}

bool checkAddress(uint64_t address) {
    return !(address & 7);
}

inline int align_address(uint32_t reladdr) {
    int rem = reladdr % 8;
    if (rem == 0)
        return reladdr;
    else
        return reladdr + (8 - rem);
}

void boot::readFramebufferTag(uint8_t* address, boot::info_t* boot_info) {
    address += 8; // skip type and size
    
    boot_info->screen.framebuffer = *((uint64_t*) address);
    address += 8; // just loaded framebuffer

    boot_info->screen.pitch = *((uint32_t*) address); address += 4;
    boot_info->screen.width = *((uint32_t*) address); address += 4;
    boot_info->screen.height = *((uint32_t*) address); address += 4;
    boot_info->screen.bpp = *((uint8_t*) address); address++;
    boot_info->screen.type = *((uint8_t*) address); address++;
}

void boot::readTag(uint8_t* address, uint32_t* type, uint32_t* size) {
    uint32_t* ptr = (uint32_t*) address;
    *type = *ptr; ptr++;
    *size = *ptr;
}

void boot::readHeader(uint8_t* address, boot::info_t* boot_info) {
    // read meta
    multiboot_basic_tag* basic_tag = (multiboot_basic_tag*) address;
    boot_info->meta.total_size = basic_tag->total_size;
    uint8_t* deadline = (address + boot_info->meta.total_size);

    address += 8;
    uint32_t type, size;
    do {
        read_multiboot2_tag(address, &type, &size);
        
        switch (type) {
            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER : {
                read_multiboot2_tag_framebuffer(address, boot_info); break;
            }
            case MULTIBOOT_TAG_TYPE_END : return;
            default: break;
        }
        
        address += align_address(size);
    } while(address < deadline);
}

void printInfo(boot::info_t* /*boot_info*/) {
    // TODO
}
