#ifndef __MEMORY_HPP__
#define __MEMORY_HPP__

#include "legacy/stdint.hpp"

namespace memory {
    enum mode_t {
        PAGED = 0
    };

    struct memory_t {
        uint64_t base;
        uint64_t limit;
        mode_t mode;
    };

    // Also, this class only serves the kernel
    // For User Mode programs you must use a strict pager
    class Manager {
        public:
            Manager();
            ~Manager();

            virtual void* malloc(uint32_t size) = 0;
            virtual void* realloc(void* ptr, uint32_t size) = 0;
            virtual void  free(void* ptr) = 0;

            virtual void  set(void* ptr, uint8_t value, uint32_t length) = 0;
            virtual void  copy(void* dst, void* src, uint32_t length) = 0;
            virtual void  move(void* dst, void* src, uint32_t length) = 0;

            virtual void  compare(void* dst, void* src, uint32_t length) = 0;
            virtual void* find(void* ptr, uint8_t value, uint32_t length) = 0;

            static Manager* buildManager(memory_t properties);
    };
};

#endif
