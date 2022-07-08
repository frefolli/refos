#include "memory/memory.hpp"
#include "memory/paged.hpp"

memory::Manager::Manager() {
    //
}

memory::Manager::~Manager() {
    //
}

memory::Manager* memory::Manager::buildManager(memory::memory_t properties) {
    if (properties.mode == memory::mode_t::PAGED) {
        uint8_t* ptr = (uint8_t*) properties.base;
        properties.base += sizeof(memory::PagedManager);
        memory::PagedManager *manager = new (ptr) memory::PagedManager(properties);
        return (memory::Manager*) manager;
    }
    return 0;
}
