;------------------------------------------------------------------------------
; Name:
;   germ
;
; in:
;   edi-address to callback function
;
; Description:
;   Code which will scan for free space in memory and copy itself into it.
;
;   This must be encoded in x86 assembly as nasm x64 will not resolve labels
;   if used as memory addresses.
;
;
[section .text]

%include "germ.mac"

global _start

_start:
    nop
    call        delta
delta:
    pop         esi
    sub         esi, delta          ; our start address

    ; print out the begin execution message
    _print      exe_str, exe_str_len

;    call        search

;    call        copy

exit:
    ; print completion message
    _print      comp_str, comp_str_len

    xor         eax, eax
    ret

;------------------------------------------------------------------------------
; print:
;       use the linux write syscall to output text string to stdout
; in:
;       ecx-address of string
;       edx-string length
; returns:
;       eax-#bytes written
;
print:
    xor         eax, eax
    xor         ebx, ebx
    mov         al, 0x4             ; sys_write
    mov         bl, 0x1             ; stdout fd
    int         0x80
    ret

;------------------------------------------------------------------------------
; search:
;
; in:
; returns:
;
search:
    ; now begin to search for enough free space
    xor         eax, eax            ; search for NULL
    mov         edi, ebp            ; start address
    add         edi, germ_len       ; start address + length
    xor         ecx, ecx
    add         ecx, germ_len       ; counter

.loop:
    add         edi, 0x1
    cmp         [edi], al
    jne         .nospace

    dec         cl
    cmp         ecx, 0x0
    jne         .loop

.nospace:
    xor         eax, eax
    mov         eax, 0x1
    jmp         .exit
.exit:
    ret

;------------------------------------------------------------------------------
; copy:
;
; in:
; returns:
;
copy:
    mov         edi, ebp
    add         edi, germ_len
    mov         edi, germ_len
    rep         movsb
    ret

;------------------------------------------------------------------------------
; fail:
;
; in:
; returns:
;
failure:
    ; print out failure message
    _print      fail_str, fail_str_len

    mov         ebx, 0xDEADBEEF
    mov         eax, 0x1

    ; exit
    ret


;------------------------------------------------------------------------------
; debug string
;
exe_str:        db "[x] germ executing",10
exe_str_len:    equ $-exe_str

comp_str:       db "[x] complete",10
comp_str_len:   equ $-comp_str

fail_str:       db "[x] failure",10
fail_str_len:   equ $-fail_str

;------------------------------------------------------------------------------
; germ info
;
stack_sz:       dw 0ffh

germ_len:       equ end-_start
end:
