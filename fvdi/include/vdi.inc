*****
* fVDI VDI declarations, by Johan Klockars
*
* This file is put in the public domain.
* It is not copyrighted or under any sort of license.
*****

CHUNKY		=	1
TRUE_COLOUR	=	2

WHITE		= 0
BLACK 		=	1
EFFECTS		=	0x3f

only_fvdi	=	1

*
* VDI array
*
control		=	0
intin		=	4
ptsin		=	8
intout		=	12
ptsout		=	16

*
* Control
*
function	=	0
L_ptsin		=	2
L_ptsout	=	4
L_intin		=	6
L_intout	=	8
subfunction	=	10
handle		=	12


*
* VDI struct
* Font struct
*
	.include	"types.inc"
