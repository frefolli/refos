#include "boot/boot.hpp"
#include "boot/multiboot2.hpp"
#include "kernel/kernel.hpp"

bool boot::checkMagic(uint64_t magic) {
    return (magic == MULTIBOOT2_BOOTLOADER_MAGIC);
}

bool boot::checkAddress(uint64_t address) {
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
        boot::readTag(address, &type, &size);
        
        switch (type) {
            case MULTIBOOT_TAG_TYPE_FRAMEBUFFER : {
                boot::readFramebufferTag(address, boot_info); break;
            }
            case MULTIBOOT_TAG_TYPE_END : break;
            default: break;
        }
        
        address += align_address(size);
    } while(address < deadline);

    boot_info->memory.base = 0x100000;
    boot_info->memory.limit = 0;
    boot_info->memory.mode = memory::mode_t::PAGED;
}

void boot::printInfo(boot::info_t* boot_info) {
    video::Adapter* out = kernel::kernel.video;

    // start
    out->printf("boot info ...\n");
    
    // meta
    out->printf("|  meta  | magic       = %d\n",
           boot_info->meta.magic);
    out->printf("|  meta  | address     = %d\n",
            boot_info->meta.address);
    out->printf("|  meta  | total_size  = %d\n",
            boot_info->meta.total_size);
    // screen
    out->printf("| screen | framebuffer = %d\n",
            boot_info->screen.framebuffer);
    out->printf("| screen | width       = %d\n",
            boot_info->screen.width);
    out->printf("| screen | height      = %d\n",
            boot_info->screen.height);
    out->printf("| screen | pitch       = %d\n",
            boot_info->screen.pitch);
    out->printf("| screen | bpp         = %d\n",
            boot_info->screen.bpp);
    out->printf("| screen | type        = %d\n",
            boot_info->screen.type);

    // memory
    out->printf("| memory | base        = %d\n",
           boot_info->memory.base);
    out->printf("| memory | limit       = %d\n",
            boot_info->memory.limit);
    out->printf("| memory | mode        = %d\n",
            boot_info->memory.mode);
    // end
    out->printf("\n");
}
