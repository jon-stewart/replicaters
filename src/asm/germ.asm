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

; stack frame defs.
%define SADDR       [rbp-0x8]       ; germ start address
%define REG_CB      [rbp-0x10]      ; registration cb address
%define DADDR       [rbp-0x18]      ; destination copy address
%define DEBUG_CB    [rbp-0x20]      ; debug cb address
%define SFRAME      0x20

global _start

_start:
    nop
    
    ; prolog - stack frame creation
    push        rbp
    mov         rbp, rsp
    sub         rsp, SFRAME

    call        delta
delta:
    pop         r15
    sub         r15, delta          ; our start address
    mov         SADDR, r15          ; store start address
    mov         REG_CB, rdi         ; store reg_cb address
    mov         DEBUG_CB, rsi       ; store debug_cb address

    ; print out the begin execution message
    _print      SADDR, exe_str, exe_str_len

    ; carry out the search for free space to replicate into
    call        search

    ; if no free space found we exit out without the copy
    test        rax, rax
    jz          complete

    ; store the copy dest address
    mov         DADDR, rax

    ; carry out the copy of this germ
    mov         rdi, DADDR          ; copy dest address
    mov         rsi, SADDR          ; germ start address
    _get_var    SADDR, germ_len, cx
    rep         movsb               ; copy those bytes

    ; print copy message
    _print      SADDR, cpy_str, cpy_str_len

    ; make call to the registration cb
    xor         rsi, rsi
    xor         rdx, rdx
    mov         rdi, DADDR          ; arg0: replicant address
    _get_var    SADDR, germ_len, si ; arg1: length
    _get_var    SADDR, gen, dl      ; arg2: generation
    call        REG_CB              ; reg_cb address

complete:
    ; print completion message
    _print      SADDR, comp_str, comp_str_len

exit:
    ; epilog - stack frame cleanup
    mov         rsp, rbp
    pop         rbp
    xor         rax, rax
    ret

;------------------------------------------------------------------------------
; print:
;   use the linux write syscall to output text string to stdout
; in:
;   rcx-address of string
;   rdx-string length
; returns:
;   rax-#bytes written
;
print:
    xor         rax, rax
    mov         al,  0x1            ; sys_write
    mov         rdi, 0x1            ; stdout fd
    syscall
    ret

;------------------------------------------------------------------------------
; search:
;   look for enough free space to replicate into.  We can only search as far
;   as our 'reach' will allow.
; in:
; returns:
;   rax-address found
;
search:
    xor         rax, rax
    xor         rdi, rdi
    _get_var    SADDR, germ_len, di
    add         rdi, SADDR
    xor         rcx, rcx
    _get_var    SADDR, reach, cx
.find_null:
    repne       scasw               ; repeat scasw as long as [rdi] is not NULL (rax)

    ; exit if we out of reach (rcx zero)
    test        rcx, rcx
    jz          .fail

    ; hold the copy dest address
    mov         r12, rdi

    ; hold the remaining reach
    mov         r13, rcx

.find_space:
    ; find enough free space for replication
    xor         rcx, rcx
    _get_var    SADDR, reach, cx
    repe        scasw               ; repeat scasw as long as [rdi] is NULL (rax)

    ; if rcx is not zero we do not have enough space
    test        rcx, rcx
    mov         rcx, r13            ; remaining reach
    jnz         .find_null          ; restart search

    ; return the copy dest address
    mov         rax, r12
    jmp         .exit

.fail:
    xor         rax, rax
.exit:
    ret

;------------------------------------------------------------------------------
; fail:
;
; in:
; returns:
;
failure:
    ; print out failure message
    _print      SADDR, fail_str, fail_str_len

    mov         rbx, 0xDEADBEEF
    mov         rax, 0x1

    pop         r15                 ; get rid of previous return address
    jmp         exit


;------------------------------------------------------------------------------
; debug string
;
%ifdef DEBUG
exe_str:        db "[x] germ executing",10,0
exe_str_len:    equ $-exe_str

cpy_str:        db "[x] copied",10,0
cpy_str_len:    equ $-cpy_str

comp_str:       db "[x] complete",10,0
comp_str_len:   equ $-comp_str

fail_str:       db "[x] failure",10,0
fail_str_len:   equ $-fail_str
%endif

;------------------------------------------------------------------------------
; germ info
;
stack_sz:       db 020h
reach:          dw 0FFh             ; how many bytes we can search
gen:            db  01h             ; generation
life:           db  05h

germ_len:       dw end-_start       ; size of germ
end:
