#ifndef __BOOT_HPP__
#define __BOOT_HPP__

#include "../video/video.hpp"
#include "../libc/stdbool.hpp"

namespace boot {
    typedef struct __attribute__((packed)) {
        uint64_t magic;
        uint64_t address;
        uint32_t total_size;
    } meta_t;

    typedef struct __attribute__((packed)) {
        meta_t meta;
        video::screen_t screen;
    } info_t;
    
    bool checkMagic(uint64_t magic);
    
    bool checkAddress(uint64_t address);

    void readHeader(uint8_t* address, info_t* boot_info);

    void printInfo(info_t* boot_info);
};

#endif
