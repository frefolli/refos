#include "kernel/kernel.hpp"
#include "legacy/stdbool.hpp"

kernel::Kernel::Kernel(boot::info_t* bootInfo) {
    this->bootInfo = bootInfo;
}

kernel::Kernel::Kernel() {
    // placeholder
}

kernel::Kernel::~Kernel() {
    // won't clean memory for now, because
    // it's prevised that kernel can be destroyed
    // in a normal situation
}

void kernel::Kernel::initVideo() {
    this->video = video::Adapter::buildAdapter(bootInfo->screen);
}

void kernel::Kernel::initMemory() {
    this->memory = memory::Manager::buildManager(bootInfo->memory);
}

void kernel::Kernel::main() {
    // start services, for now just print boot_info
    boot::printInfo(this->bootInfo);
    this->panic("nothing else to do, panik!");
}

void kernel::Kernel::die() {
    while(true);
}

void kernel::Kernel::panic(const char* reason) {
    this->video->printf("kernel panic: %s\n", reason);
    this->die();
}
