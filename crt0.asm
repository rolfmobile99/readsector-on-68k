;
; crt0.asm - for "68k-nano" board
;
; mods by Rolf:
;
; - in "_start" code, switch to USER mode (from PRIVILEGED MODE)
;       for this, we need to choose where the user stack ptr will be (see INITIAL_USP in memory.inc)
;

        include "68k-nano/memory.inc"
        include "68k-nano/syscalls.inc"

        ; indicate that code here goes in the ".text" segment - this is absolutely required!!
        section .text

        
;        global start,main
        global _start,_exit

;        org     APPMEMSTART     ; XXX we want the "ld" to specify this!

_start:
        nop                     ; assume we're still in supervisor mode (or this will crash!)
        move.l  #INITIAL_USP,a0 ; setup user stack pointer
        move.l  a0,usp          ;
        move.w  #$0000,sr       ; then, enter user mode. (S bit = 0)

        bsr     main            ; go to main

_exit:
        nop
        sys Exit                ; return to ROM system

