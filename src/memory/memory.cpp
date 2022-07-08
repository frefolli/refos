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
        memory::Manager* ptr = (memory::Manager*) properties.base;
        properties.base += sizeof(memory::PagedManager);
        *ptr = memory::PagedManager(properties);
    }
    return 0;
}
