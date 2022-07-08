#include "boot/boot.hpp"
#include "boot/multiboot2.hpp"

inline int align_address(uint32_t reladdr) {
    int rem = reladdr % 8;
    if (rem == 0)
        return reladdr;
    else
        return reladdr + (8 - rem);
}

void read_multiboot2_tag_framebuffer(uint8_t* /*tag*/, boot_info_t* /*boot_info*/) {
}

extern void print_couple(uint64_t A, uint64_t B);

void read_multiboot2_tag(uint8_t* address, uint32_t* type, uint32_t* size) {
    uint32_t* ptr = (uint32_t*) address;
    *type = *ptr; ptr++;
    *size = *ptr;
}

void read_multiboot2_header(uint8_t* address, boot_info_t* boot_info) {
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
                print_couple(type, size);
                read_multiboot2_tag_framebuffer(address, boot_info); break;
            }
            case MULTIBOOT_TAG_TYPE_END : return;
            default: break;
        }
        
        address += align_address(size);
    } while(address < deadline);
};
