#include "interrupts/isr.hpp"
#include "kernel/kernel.hpp"

void isr_handler(int intNum, registers_t* /*regs*/) {
    kernel::kernel.video->printf("k_int: %d\n", intNum);
}


