;------------------------------------------------------------------------------
; Name:
;   germ
;
; Description:
;   Code which will scan for free space in memory and copy itself into it.
;
;   Ensure no NULL bytes in output machine code
;
[section .text]

%include "sys_calls.mac"            ; include my syscall macros

global _start

_start:
    nop                             ; keep gdb happy

    jmp         short string
code:

    pop         esi                 ; address of string
    sys_write   1, esi, str_len     ; fd, buff, len

    ret

string:

    call        code
str:
    db          "Germ executing",10
str_len:        equ $-str

file_len:       equ end-_start
end:
