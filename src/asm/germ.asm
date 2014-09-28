;------------------------------------------------------------------------------
; Name:
;   germ x64
;
; in:
;   edi-address to callback function
;
; Description:
;   Code which will scan for free space in memory and copy itself into it.
;
[section .text]

[BITS 64]

%include "germ.mac"

global _start

_start:
    nop
    
    ; prolog - stack frame creation
    push        rbp
    mov         rbp, rsp
    sub         rsp, 0x18
    ; stack frame:
    ;   - 0x8  start address
    ;   - 0x10 cb address
    ;   - 0x18 copy address

    call        delta
delta:
    pop         r15
    sub         r15, delta          ; our start address
    mov         [rbp-0x8], r15      ; store start address
    mov         [rbp-0x10], rdi     ; store cb address

    ; print out the begin execution message
    _print      0x8, exe_str, exe_str_len

    call        search
    mov         [rbp-0x18], rax

    mov         rdi, [rbp-0x18]
    call        copy

    ; print completion message
    _print      0x8, comp_str, comp_str_len

    ; make call to the callback
    mov         rdi, [rbp-0x18]     ; arg0: address
    mov         rsi, germ_len       ; arg1: length
    call        [rbp-0x10]          ; cb address


exit:
    ; epilog - stack frame cleanup
    mov         rsp, rbp
    pop         rbp
    xor         rax, rax
    ret

;------------------------------------------------------------------------------
; print:
;       use the linux write syscall to output text string to stdout
; in:
;       rcx-address of string
;       rdx-string length
; returns:
;       rax-#bytes written
;
print:
    xor         rax, rax
    mov         al,  0x1            ; sys_write
    mov         rdi, 0x1            ; stdout fd
    syscall
    ret

;------------------------------------------------------------------------------
; search:
;       now begin to search for enough free space
; in:
; returns:
;       rax-address found
;
search:
    xor         rax, rax
    mov         rdi, [rbp-0x8]      ; start address
    add         rdi, germ_len       ; move pointer to beyond end
.find_null:
    xor         rcx, rcx
    add         rcx, reach          ; area in which we can replicate
    repne       scasw               ; repeat scasw as long as [rdi] is not NULL (rax)

    ; exit if we out of reach (rcx zero)
    test        rcx, rcx
    jz          .exit

    ; temporarily store the copy start address
    mov         r12, rdi

.find_space:
    xor         rcx, rcx
    add         rcx, germ_len
    repe        scasw               ; repeat scasw as long as [rdi] is NULL (rax)

    ; if rcx is not zero we do not have enough space
    test        rcx, rcx
    jnz         .find_null

    ; return the copy start address
    mov         rax, r12
.exit:
    ret

;------------------------------------------------------------------------------
; copy:
;
; in:
;       rdi-start address
; returns:
;
copy:
    mov         rsi, [rbp-0x8]      ; germ start address
    mov         rcx, germ_len
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
    _print      0x8, fail_str, fail_str_len

    mov         rbx, 0xDEADBEEF
    mov         rax, 0x1

    pop         r15                 ; get rid of previous return address
    jmp         exit


;------------------------------------------------------------------------------
; debug string
;
exe_str:        db "[x] germ executing",10,0
exe_str_len:    equ $-exe_str

comp_str:       db "[x] complete",10,0
comp_str_len:   equ $-comp_str

fail_str:       db "[x] failure",10,0
fail_str_len:   equ $-fail_str

;------------------------------------------------------------------------------
; germ info
;
stack_sz:       equ 0ffh
reach:          equ 080h

germ_len:       equ end-_start
end:
