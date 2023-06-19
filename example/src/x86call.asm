section .text
    %include "include/x86subcall_inc.asm"
    global x86call
    x86call:
        mov eax, edi
        imul eax, eax
        call x86subcall
        ret
