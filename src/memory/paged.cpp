#include "memory/paged.hpp"

memory::PagedManager::PagedManager(memory_t properties) {
    this->properties = properties;
    this->pointer = (void*) (this->properties.base);
}

memory::PagedManager::~PagedManager() {
    //
}

void* memory::PagedManager::malloc(uint32_t size) {
    void* ptr = this->pointer;
    this->pointer = (void*) ((uint8_t*) (this->pointer) + size);
    return ptr;
}

void* memory::PagedManager::realloc(void* ptr, uint32_t /*size*/) {
    return ptr;
}

void  memory::PagedManager::free(void* /*ptr*/) {
    //
}

void  memory::PagedManager::set(
        void* /*ptr*/, uint8_t /*value*/, uint32_t /*length*/) {
    //
}

void  memory::PagedManager::copy(
        void* /*dst*/, void* /*src*/, uint32_t /*length*/) {
    //
}

void  memory::PagedManager::move(
        void* /*dst*/, void* /*src*/, uint32_t /*length*/) {
    //
}

void  memory::PagedManager::compare(
        void* /*dst*/, void* /*src*/, uint32_t /*length*/) {
    //
}

void* memory::PagedManager::find(
        void* ptr, uint8_t /*value*/, uint32_t /*length*/) {
    return ptr;
}
