	xdef	__mint_setjmp
	xdef	__mint_longjmp

	text

__mint_setjmp:
	move.l	4(sp),a0		; Address of jmp_buf[]
	move.l	(sp),(a0)		; Save return address
	movem.l	d2-d7/a2-a7,4(a0)	; Save registers d2-d7/a2-a7
	clr.l	d0
	rts


__mint_longjmp:
	move.l	4(sp),a0		; Address of jmp_buf[]
;
; this function is only used at a few places in the
; freetype library, and we can safely always use '1' here
;
	moveq	#1,d0		; Value to return
	movem.l	4(a0),d2-d7/a2-a7	; Restore saved reggies
	move.l	(a0),(sp)		;  and the saved return address
	rts

	end
