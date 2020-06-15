  .ifdef	xbra_chain
*	.external	_vdi_address
  .endc

  .ifdef	_stand_alone
*	xref	_stand_alone
  .endc


	.macro	save_regs
	movem.l	a1-a2,-(a7)
	.endm

	.macro	uses_d1
  .if	transparent
	move.l	d1,-(a7)
  .endif
	.endm

	.macro	used_d1
  .if	transparent
	move.l	(a7)+,d1
  .endif
	.endm

	.macro	restore_regs
	movem.l	(a7)+,a1-a2
	.endm

	.macro	real_return
	restore_regs
	rte
	.endm

	.macro	done_return
  .ifne	only_fvdi
	real_return
  .else
	return
  .endif
	.endm

	.macro	ijsr indirect
  .if mc68000 == 1
	pea	ret\@
	move.l	\indirect,-(a7)
	rts
ret\@:
  .else
	jsr	([\indirect])
  .endif
	.endm

	.macro	ijmp indirect
  .ifne mc68000 == 1
	move.l	\indirect,-(a7)
	rts
  .else
	jmp	([\indirect])
  .endif
	.endm


	.macro	redir number
  .if 1
	beq	redirect
  .else
	.globl	_puts,_ltoa
	bne	noredirect\@
	movem.l	d0-d2/a0-a2,-(a7)
	bra	cont\@
text\@:
	.ascii	"redirect "
	.byte	0
eol\@:
	.byte	10,13,0,0
	.bss
numbuf\@:
	ds.b	10
	.text
cont\@:
	pea	text\@
	jsr	_puts
	addq.l	#4,a7
	move.l	#16,-(a7)
	move.l	#\number,-(a7)
	pea	numbuf\@
	jsr	_ltoa
	add.w	#12,a7
	pea	numbuf\@
	jsr	_puts
	addq.l	#4,a7
	pea	eol\@
	jsr	_puts
	addq.l	#4,a7
	movem.l	(a7)+,d0-d2/a0-a2
	bra	redirect
noredirect\@:
  .endc
	.endm
