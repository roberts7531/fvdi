/*
 * small test program. constantly outputs the mouse cursor
 * position using nf_stderr natfeat call.
 * known bugs:
 * - blindly installs an interrupt routine in vblqueue[1]
 */

GCURX = -0x25a
PLANES = 0
BYTES_LIN = -2

vblqueue = 0x456
v_bas_ad = 0x44e

init:
	movea.l    4(a7),a0
	move.l     a0,_BasPag
	lea.l      stacktop,a7
	move.l     12(a0),d0
	add.l      20(a0),d0
	add.l      28(a0),d0
	addi.l     #256,d0
	move.l     d0,d7
	move.l     d0,-(a7)
	move.l     a0,-(a7)
	clr.w      -(a7)
	move.w     #0x004A,-(a7)
	trap       #1
	lea.l      12(a7),a7

	.dc.w 0xa000
	movem.l    a0-a2,linea_vars
	move.l a0,a5

	pea nf_stderr(pc)
	bsr nf_getid
	addq.w #4,a7
	move.l d0,nf_stderr_id
	beq  fail
	
	pea init_intr(pc)
	move.w #38,-(a7)
	trap #14
	addq.l     #6,a7

	clr.w      -(a7)
	move.l     d7,-(a7)
	move.w     #49,-(a7) /* Ptermres */
	trap       #1

fail:
	clr.w -(a7) /* Pterm0 */
	trap #1
	
init_intr:
	move.l (vblqueue).w,a0
	lea    vbl_intr(pc),a1
	move.l a1,4(a0)
	rts

vbl_intr:
	move.l linea_vars(pc),a0
	lea.l  lastpos(pc),a1
	move.l GCURX(a0),d0
	cmp.l  (a1),d0
	beq vbl_intr_ex
	move.l d0,(a1)
	lea msgbuf+9(pc),a1

    bsr    print_digit
    bsr    print_digit
    bsr    print_digit
    bsr    print_digit
    move.b #' ',-(a1)
    swap   d0
    bsr    print_digit
    bsr    print_digit
    bsr    print_digit
    bsr    print_digit

    move.l a1,-(a7)
    move.l nf_stderr_id(pc),-(a7)
    bsr.s  nf_call
    addq.w #8,a7
    
vbl_intr_ex:
	rts

print_digit:
	moveq  #0,d4
	move.w d0,d4
	divu.w #10,d4
	move.w d4,d0
	swap   d4
	add.b  #'0',d4
	move.b d4,-(a1)
	rts

nf_call:
	.dc.w 0x7301
	rts
	
nf_getid:
	.dc.w 0x7300
	rts
	
	.data
		
lastpos: dc.l -1
nf_stderr: .dc.b "NF_STDERR",0
	.even
msgbuf: dc.b "         ",13,0
	.even

	.bss
_BasPag:            ds.l 1

nf_stderr_id: ds.l 1

/* return values from linea_init, must be in order */
linea_vars:         ds.l 1
linea_fonts:        ds.l 1
linea_fcts :        ds.l 1

stack: ds.b 256
stacktop: ds.l 1
