;
; cfcard.asm - low-level interface to CF Card for 68k-nano board + 68k nano's ROM
;

; C API:
;       (see cfcard.h)
;
;       int cf_readsector(int secnum, char *buf, int bufsize) - read one sector
;
;       params:
;           secnum - sector number (i.e. LBA)
;           buf - bytes read are returned in this buffer, must be word-aligned
;           bufsize - number of bytes to read (e.g. 512)
;       returns:
;           0 = success, otherwise error code 
;

	include "68k-nano/syscalls.inc"     ; bring in TRAPs for 68k-nano ROM

    ; 'public' - this puts a "T" entry in the symbol table of the .o file.

	public	cf_readsector

cf_readsector:
	move.l 4(sp),d0     ; sector#
	move.l 8(sp),a0     ; ptr to read buffer
	move.l 12(sp),d1    ; #bytes to read

	sys ReadSector      ; calls TRAP to output one char (to UART)
	                    ; return code in d0
	rts
