#ifndef __KERNEL_HPP__
#define __KERNEL_HPP__

#include "video/video.hpp"
#include "memory/memory.hpp"
#include "boot/boot.hpp"

namespace kernel {
    class Kernel {
        private:
            video::Adapter* VIDEO;
            memory::Manager* MEMORY;
        public:
            Kernel(boot::info_t* boot_info);
            ~Kernel();

            void main();
    };

    extern Kernel kernel;
};

#endif
