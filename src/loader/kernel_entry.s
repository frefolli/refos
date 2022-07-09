global _kernel_entry
bits 64

section .text

extern MAGIC
extern ADDRESS
extern kernel_end

_kernel_entry:
    mov rdi, [MAGIC]
    mov rsi, [ADDRESS]
    mov rdx, kernel_end
    
    extern kmain
    call kmain
