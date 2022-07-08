#ifndef __KERNEL_HPP__
#define __KERNEL_HPP__

#include "video/video.hpp"
#include "memory/memory.hpp"
#include "boot/boot.hpp"

namespace kernel {
    class Kernel {
        public:
            video::Adapter* video;
            memory::Manager* memory;
            boot::info_t* bootInfo;

            Kernel(boot::info_t* boot_info);
            Kernel(); // empty
            ~Kernel();

            void main();
            void die();
            void panic(const char*);
    };

    extern Kernel kernel;
};

#endif
