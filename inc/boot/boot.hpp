#ifndef __BOOT_HPP__
#define __BOOT_HPP__

#include "../video/video.hpp"
#include "../memory/memory.hpp"
#include "../legacy/stdbool.hpp"

namespace boot {
    typedef struct __attribute__((packed)) {
        uint64_t magic;
        uint64_t address;
        uint32_t total_size;
    } meta_t;

    typedef struct __attribute__((packed)) {
        meta_t meta;
        video::screen_t screen;
        memory::memory_t memory;
    } info_t;
    
    bool checkMagic(uint64_t magic);
    
    bool checkAddress(uint64_t address);

    void readHeader(uint8_t* address, info_t* boot_info);

    void readFramebufferTag(
            uint8_t* address, boot::info_t* boot_info);

    void readTag(
            uint8_t* address, uint32_t* type, uint32_t* size);

    void printInfo(info_t* boot_info);
};

#endif
