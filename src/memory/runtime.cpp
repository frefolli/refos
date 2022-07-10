#include "kernel/kernel.hpp"

void* operator new(size_t size) {
    return kernel::kernel.memory->malloc(size);
}

void* operator new[](size_t size) {
    return kernel::kernel.memory->malloc(size);
}

void operator delete(void* p) {
    kernel::kernel.memory->free(p);
}

void operator delete(void* p, size_t) {
    ::operator delete(p);
}

void operator delete[](void* p) {
    kernel::kernel.memory->free(p);
}

void operator delete[](void* p, size_t) {
    ::operator delete[](p);
}

extern "C" void __cxa_pure_virtual() {
    kernel::kernel.panic("__cxa_pure_virtual");
}
