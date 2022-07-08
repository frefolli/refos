#ifndef __MEMORY_HPP__
#define __MEMORY_HPP__

namespace {
    struct memory_t {
        uint64_t base;
        uint64_t limit;
    }

    enum mode_t {
        PAGED = 0
    }

    // This is the only class to be initialized
    // with thing on stack, because it enables
    // dynamic memory allocation and deallocation
    // 
    // Also, this class only serves the kernel
    // For User Mode programs you must use a strict pager
    class Manager {
        public:
            Manager(memory_t memory);
            virtual ~Manager(memory_t memory);

            virtual void* malloc(uint32_t size);
            virtual void* realloc(void* ptr, uint32_t size);
            virtual void  free(void* ptr);

            virtual void  set(void* ptr, uint8_t value, uint32_t length);
            virtual void  copy(void* dst, void* src, uint32_t length);
            virtual void  move(void* dst, void* src, uint32_t length);

            virtual void  compare(void* dst, void* src, uint32_t length);
            virtual void* find(void* ptr, uint8_t value, uint32_t length);

            static Manager* buildManager(memory_t properties);
    }
}

#endif
