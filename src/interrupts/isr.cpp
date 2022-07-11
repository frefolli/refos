#include "interrupts/isr.hpp"
#include "kernel/kernel.hpp"

isr_t interruptHandlers[256];

void interrupts::registerHandler(uint8_t n, isr_t handler) {
  interruptHandlers[n] = handler;
}

void isr_handler(int intNum, registers_t* /*regs*/) {
    kernel::kernel.video->printf("k_int: %d\n", intNum);
}

inline void outb (unsigned short _port, unsigned char _data) {
    asm __volatile__ ("outb %0, %1" : : "a" (_data), "dN" (_port));
}

void irq_handler(int intNum, registers_t* regs) {
    // Send an EOI (end of interrupt) signal to the PICs.
   // If this interrupt involved the slave.
   if (intNum >= 40) {
       // Send reset signal to slave.
       outb(0xA0, 0x20);
   }
   // Send reset signal to master. (As well as slave, if necessary).
   outb(0x20, 0x20);

   if (interruptHandlers[intNum] != 0) {
       isr_t handler = interruptHandlers[intNum];
       handler(regs);
   }
}
