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

    pop         edx                 ; address of string
    sys_write   1, edx, str_len     ; fd, buff, len

    ret

string:

    call        code
str:
    db          "Germ executing",10,0
str_len:        equ $-str

len:            equ end - _start
end:
