;
; putchar.asm - ASM version of putchar for 68k-nano board + 68k nano's ROM
;

; C API:
;      int putchar(int c)
;

	include "68k-nano/syscalls.inc"     ; bring in TRAPs for 68k-nano ROM

    ; note that vasm docs say ".global" ... 
    ; this puts a "T" entry in the symbol table of the .o file.

	global	putchar
	global __modsi3,__divsi3,__udivsi3,__mulsi3

; 	.type	putchar, @function
putchar:
	move.l 4(sp),d0
	sys OutChar      ; calls TRAP to output one char (to UART)
	rts


; borrow this code from "factor1c.tricky.asm" (original from divtest)
;
; functions __divsi3, __modsi3, __mulsi3
;

; XXX lightly tested
__modsi3:
	move.l	8(sp),d1	; /* d1 = divisor */
	move.l	4(sp),d0	; /* d0 = dividend */
	move.l	d1,-(sp)
	move.l	d0,-(sp)
	bsr __divsi3
	addq.l	#8,sp
	move.l	8(sp),d1	; /* d1 = divisor */

	move.l	d1,-(sp)
	move.l	d0,-(sp)
	bsr __mulsi3        ; /* d0 = (a/b)*b */
	addq.l	#8,sp

	move.l	4(sp),d1	; /* d1 = dividend */
	sub.l	d0,d1		; /* d1 = a - (a/b)*b */
	move.l	d1,d0
	rts


;
; a div b
; this essentially checks both arguments for their signs, and remembers that, and
; makes both arguments non-negative. (i.e abs value)
; then it calls unsigned div.
; finally, the result is corrected for sign.
;
__divsi3:
	move.l	d2,-(sp)    ; preserve d2
	moveq	#1,d2	    ;/* sign of result stored in d2 (=1 or =-1) */
	move.l	12(sp),d1	;/* d1 = divisor */
	bge	.Z1             ; note: was jpl (jump if plus?)
	neg.l	d1
	neg.b	d2		    ;/* change sign because divisor <0  */
.Z1:
	move.l	8(sp),d0	;/* d0 = dividend */
	bge	.Z2
	neg.l	d0
	neg.b	d2
.Z2:
	move.l	d1,-(sp)
	move.l	d0,-(sp)
	bsr  __udivsi3      ;/* divide abs(dividend) by abs(divisor) */
	addq.l	#8,sp
	tst.b	d2
	bge	.Z3
	neg.l	d0          ; result is d0, negate if needed
.Z3:
	move.l	(sp)+,d2   ; restore d2
	rts


; Note: only somewhat tested... logic below .L4 not well tested yet
;
; a div b  (unsigned)
;
__udivsi3:
	move.l	d2,-(sp)    ; preserve d2

	move.l	12(sp),d1	;/* d1 = divisor (b) */
	move.l	8(sp),d0	;/* d0 = dividend (a) */
	cmp.l	#$10000,d1  ;/* divisor (b) >= 2 ^ 16 ? (i.e. d1 - 0x10000 )*/
	bcc	 .U3		       ;/* then try next algorithm */ - could this be 'bge' ??
	move.l	d0,d2
	clr.w	d2
	swap	d2
	divu.w	d1,d2       ;/* divide - high quotient in lower word */
	move.w	d2,d0		;/* save high quotient */
	swap	d0
	move.w	10(sp),d2	;/* get low dividend (a-low) + high rest */
	divu.w	d1,d2		;/* divide - low quotient */
	move.w	d2,d0
	bra	 .U6
.U3:
	move.l	d1,d2		;/* use d2 as divisor (b) backup */
.U4:
	lsr.l	#1,d1	    ;/* shift divisor */
	lsr.l	#1,d0	    ;/* shift dividend */
	cmp.l	#$10000,d1  ;/* still divisor >= 2 ^ 16 ?  */
	bcc	 .U4

	divu.w	d1,d0		;/* now we have 16-bit divisor */
	and.l	#$ffff,d0   ;/* mask out quotient, ignore remainder */
                        ;/* Multiply the 16-bit tentative quotient with the 32-bit divisor. Because of
                        ; the operand ranges, this might give a 33-bit product.  If this product is
                        ; greater than the dividend, the tentative quotient was too large. */
	move.l	d2,d1
	mulu.w	d0,d1		;/* low part, 32 bits */
	swap	d2
	mulu.w	d0,d2		;/* high part, at most 17 bits */
	swap	d2		    ;/* align high part with low part */
	tst.w	d2		    ;/* high part 17 bits? */
	bne	 .U5		        ;/* if 17 bits, quotient was too large */
	add.l	d2,d1		;/* add parts */
	bcs	 .U5		        ;/* if sum is 33 bits, quotient was too large */
	cmp.l	8(sp),d1	;/* compare the sum with the dividend */
	bls	 .U6		        ;/* if sum > dividend, quotient was too large */
.U5:
	subq.l	#1,d0	    ;/* adjust quotient */
.U6:
	move.l	(sp)+,d2    ; restore d2
	rts


;
; code segment below was adapted from original libgcc code, copyright here:
;   libgcc routines for 68000 w/o floating-point hardware.
;   Copyright (C) 1994, 1996, 1997, 1998, 2008, 2009 Free Software Foundation, Inc.
;   This file is part of GCC.
;
__mulsi3:
	move.w	4(sp),d0	;/* x0 -> d0 */
	mulu.w	10(sp),d0	;/* x0*y1 */
	move.w	6(sp),d1	;/* x1 -> d1 */
	mulu.w	8(sp),d1	;/* x1*y0 */
	add.w	d1,d0
	swap	d0
	clr.w	d0
	move.w	6(sp),d1	;/* x1 -> d1 */
	mulu.w	10(sp),d1	;/* x1*y1 */
	add.l	d1,d0
	rts
