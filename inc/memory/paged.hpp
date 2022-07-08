#ifndef __MEMORY_PAGED_HPP__
#define __MEMORY_PAGED_HPP__

#include "memory/memory.hpp"

namespace memory {
    class PagedManager : public Manager {
        private:
            memory_t properties;
        public:
            PagedManager(memory_t properties);
            ~PagedManager();

            void* malloc(uint32_t size);
            void* realloc(void* ptr, uint32_t size);
            void  free(void* ptr);

            void  set(void* ptr, uint8_t value, uint32_t length);
            void  copy(void* dst, void* src, uint32_t length);
            void  move(void* dst, void* src, uint32_t length);

            void  compare(void* dst, void* src, uint32_t length);
            void* find(void* ptr, uint8_t value, uint32_t length);
    };
};

#endif
