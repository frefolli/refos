section .multiboot_header

; %include "screen.cfg"

; Multiboot2 Header
; https://nongnu.askapache.com/grub/phcoder/multiboot.pdf
header_start:

check_start:
align 8
    dd 0xe85250d6                ; magic number
    dd 0                         ; protected mode code
    dd header_end - header_start ; header length

    ; assert  checksum
    dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))
check_end:

;framebuffer_start:
;align 8
;    dw 5             ; Type - framebuffer
;    dw 0             ; Flags
;    dd framebuffer_end - framebuffer_start
;    dd SCREEN_WIDTH  ; Width
;    dd SCREEN_HEIGHT ; Height
;    dd SCREEN_DEPTH  ; Depth
;framebuffer_end:

endtag_start:
align 8
    dw 0
    dw 0
    dd endtag_end - endtag_start
endtag_end:

header_end:
