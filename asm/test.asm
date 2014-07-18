;------------------------------------------------------------------------------
; test
;
[section .text]

%include "sys_calls.mac"            ; include my syscall macros

global _start

_start:
    nop                             ; keep gdb happy

    jmp         short string
code:

    pop         edx
    sys_write   1, edx, 10          ; fd, buff, len

    ret

string:

    call        code
    db          "hi there",10,0

len:            equ end - _start
end:
