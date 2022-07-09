#include "kernel/kernel.hpp"
#include "legacy/stdbool.hpp"
#include "memory/paged.hpp"
#include "video/vga_text.hpp"

kernel::Kernel::Kernel(boot::info_t* bootInfo) {
    this->bootInfo = bootInfo;
}

kernel::Kernel::Kernel() {
    // placeholder
}

kernel::Kernel::~Kernel() {
    // kernel won't collect garbage
}

void kernel::Kernel::main() {
    // start services, for now just print boot_info
    this->memory = memory::Manager::buildManager(bootInfo->memory);
    this->video = video::Adapter::buildAdapter(bootInfo->screen);
    
    // visualize state
    this->video->clearScreen();
    this->dumpState();

    // panic
    this->panic("shutting down the system as requested.");
}

void kernel::Kernel::dumpState() {
    // for now add this constraint
    video::VGATextAdapter* videoptr = (video::VGATextAdapter*) this->video;
    memory::PagedManager* memoryptr = (memory::PagedManager*) this->memory;
    videoptr->printf("kernel dumps the state ...\n\n");
    
    // meta
    videoptr->printf("| meta   >> magic    >> %x\n", this->bootInfo->meta.magic);
    videoptr->printf("| meta   >> address  >> %x\n", this->bootInfo->meta.address);
    videoptr->printf("| meta   >> size     >> %x\n", this->bootInfo->meta.total_size);

    // video
    videoptr->printf("| video  >> address  >> %x\n", videoptr->properties.framebuffer);
    videoptr->printf("| video  >> width    >> %d\n", videoptr->properties.width);
    videoptr->printf("| video  >> height   >> %d\n", videoptr->properties.height);
    videoptr->printf("| video  >> bpp      >> %d\n", videoptr->properties.bpp);
    videoptr->printf("| video  >> pitch    >> %x\n", videoptr->properties.pitch);
    videoptr->printf("| video  >> type     >> %d\n", videoptr->properties.type);
    videoptr->printf("| video  >> cursor.y >> %d\n", videoptr->x);
    videoptr->printf("| video  >> cursor.x >> %d\n", videoptr->y);
    videoptr->printf("| video  >> fore     >> %d\n", videoptr->foreground);
    videoptr->printf("| video  >> back     >> %d\n", videoptr->background);

    // memory
    videoptr->printf("| memory >> base     >> %x\n", memoryptr->properties.base);
    videoptr->printf("| memory >> limit    >> %x\n", memoryptr->properties.limit);
    videoptr->printf("| memory >> mode     >> %d\n", memoryptr->properties.mode);
    videoptr->printf("| memory >> pointer  >> %x\n", memoryptr->pointer);
    videoptr->printf("\n\n");
}

void kernel::Kernel::die() {
    while(true);
}

void kernel::Kernel::panic(const char* reason) {
    this->video->printf("kernel panic: %s\n", reason);
    this->die();
}
