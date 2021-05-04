;
; getchar.asm - ASM code for getchar and fgets for 68k-nano board + 68k nano's ROM
;

; C API:
;       int getchar()
;
;       char *fgets(char *str, int size, FILE *stream)
;
; LIMITATIONS:
;       no buffering is done! and 'stream' parameter is completely ignored!
;
;       fgets will return NULL upon entering an empty string (this is a limitation of PromptStr!)
;           (it *should* only return NULL when an EOF condition occurs)
;
;       also, SIMULATOR mode is NOT supported yet
;           (this could be done by implementing fgets purely with getchar)
;

	include "68k-nano/syscalls.inc"     ; bring in TRAPs for 68k-nano ROM

    ; note that vasm docs say ".global" ... but need to use "public".
    ; this puts a "T" entry in the symbol table of the .o file.

	public	getchar,fgets


getchar:
    clr.l   d0          ; ensure upper bits are clear since InChar only writes lower byte
    sys     InChar
    cmp.b   #$03,d0     ; is it crtl-C?
    beq     .isctrlchar
    cmp.b   #$04,d0     ; is it crtl-D?
    bne     .fin
.isctrlchar:
    moveq   #-1,d0      ; return EOF
.fin
    rts                 ; otherwise, character is returned in d0


fgets:
    move.l  4(sp),a0    ; get str
    move.l  8(sp),d0    ; get size
    moveq   #$0a,d1     ; set delimiter to LF (and assume echo-on, ctrl-chars "ok")
    sys     PromptStr
    moveq   #$0a,d0     ; hack to echo delimiter char (PromptStr doesn't do it)
    sys     OutChar     ; note: trashes a1, but not a0
    cmp.b   #0,(a0)     ; if empty string
    bne     .retstr
    moveq   #0,d0       ;   then return NULL
    bra     .ret
.retstr    
    move.l  a0,d0       ; return modified string
.ret
    rts





