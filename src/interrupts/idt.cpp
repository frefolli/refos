#include "interrupts/idt.hpp"
#include "kernel/kernel.hpp"

idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;

namespace interrupts {
    void setIDTGate(uint8_t num, uint64_t addr, uint16_t sel, uint8_t flags) {
       idt_entries[num].base_low = addr & 0xFFFF;
       idt_entries[num].base_med = (addr >> 16) & 0xFFFF;
       idt_entries[num].base_high = (addr >> 32);
       idt_entries[num].sel     = sel;
       idt_entries[num].null = 0;
       idt_entries[num].ist = 0;
       // We must uncomment the OR below when we get to using user-mode.
       // It sets the interrupt gate's privilege level to 3.
       idt_entries[num].flags   = flags /* | 0x60 */;
    }

    void flushIDT(uint64_t addr) {
        __asm__ ("lidt %0" :: "m"(*(idt_ptr_t*)addr));
    }
    
    void initIDT() {
        idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
        idt_ptr.base  = (uint64_t)&idt_entries;

        kernel::kernel.memory->set(&idt_entries, 0, sizeof(idt_entry_t) * 256);

        setIDTGate(0, (uint64_t) isr0, 0x08, 0x8E);
        setIDTGate(1, (uint64_t) isr1, 0x08, 0x8E);
        setIDTGate(2, (uint64_t) isr2, 0x08, 0x8E);
        setIDTGate(3, (uint64_t) isr3, 0x08, 0x8E);
        setIDTGate(4, (uint64_t) isr4, 0x08, 0x8E);
        setIDTGate(5, (uint64_t) isr5, 0x08, 0x8E);
        setIDTGate(6, (uint64_t) isr6, 0x08, 0x8E);
        setIDTGate(7, (uint64_t) isr7, 0x08, 0x8E);
        setIDTGate(8, (uint64_t) isr8, 0x08, 0x8E);
        setIDTGate(9, (uint64_t) isr9, 0x08, 0x8E);
        setIDTGate(10, (uint64_t) isr10, 0x08, 0x8E);
        setIDTGate(11, (uint64_t) isr11, 0x08, 0x8E);
        setIDTGate(12, (uint64_t) isr12, 0x08, 0x8E);
        setIDTGate(13, (uint64_t) isr13, 0x08, 0x8E);
        setIDTGate(14, (uint64_t) isr14, 0x08, 0x8E);
        setIDTGate(15, (uint64_t) isr15, 0x08, 0x8E);
        setIDTGate(16, (uint64_t) isr16, 0x08, 0x8E);
        setIDTGate(17, (uint64_t) isr17, 0x08, 0x8E);
        setIDTGate(18, (uint64_t) isr18, 0x08, 0x8E);
        setIDTGate(19, (uint64_t) isr19, 0x08, 0x8E);
        setIDTGate(20, (uint64_t) isr20, 0x08, 0x8E);
        setIDTGate(21, (uint64_t) isr21, 0x08, 0x8E);
        setIDTGate(22, (uint64_t) isr22, 0x08, 0x8E);
        setIDTGate(23, (uint64_t) isr23, 0x08, 0x8E);
        setIDTGate(24, (uint64_t) isr24, 0x08, 0x8E);
        setIDTGate(25, (uint64_t) isr25, 0x08, 0x8E);
        setIDTGate(26, (uint64_t) isr26, 0x08, 0x8E);
        setIDTGate(27, (uint64_t) isr27, 0x08, 0x8E);
        setIDTGate(28, (uint64_t) isr28, 0x08, 0x8E);
        setIDTGate(29, (uint64_t) isr29, 0x08, 0x8E);
        setIDTGate(30, (uint64_t) isr30, 0x08, 0x8E);
        setIDTGate(31, (uint64_t) isr31, 0x08, 0x8E);

        flushIDT((uint64_t)&idt_ptr);
    }
}
