*****
* fVDI linea declarations, by Johan Klockars
*
* This file is put in the public domain.
* It's not copyrighted or under any sort of license.
*****

COLBIT0	=	0x18
COLBIT1	=	0x1a
COLBIT2	=	0x1c
COLBIT3	=	0x1e
COLBIT4	=	0x656
COLBIT5	=	0x658
COLBIT6	=	0x65a
COLBIT7	=	0x65c

X1	=	0x26
Y1	=	0x28
X2	=	0x2a
Y2	=	0x2c

LSTLIN	=	0x20
LNMASK	=	0x22
WMODE	=	0x24

* mouse x-hot-spot
M_POS_HX = -0x358
* mouse y-hot-spot
M_POS_HY = -0x356
* number of planes (always 1)
M_PLANES = -0x354
* background color (pixel value)
M_CDB_BG = -0x352
* foreground color (pixel value)
M_CDB_FG = -0x350
* 32 words of mask data
MASK_FORM = -0x34e
* 45 words
DEV_TAB = -0x2b4
* mouse x-pos from bios
GCURX = -0x25a
* mouse y-pos from bios
GCURY = -0x258
* hide counter of mouse cursor
M_HID_CNT = -0x256
* mouse button state
MOUSE_BT = -0x254
* move flag and state of mouse buttons
CUR_MS_STAT = -0x15c
* visibility of text cursor
V_HID_CNT = -0x15a
* mouse x-pos for vbl handler
CUR_X = -0x158
* mouse y-pos for vbl handler
CUR_Y = -0x156
* flag for drawing mouse cursor
CUR_FLAG = -0x154
* flag for mouse cursor
MOUSE_FLAG = -0x153
* text cursor blink frequency
V_PERIOD = -0x018
* horizontal resolution
V_REZ_HZ = -0x00c
* alpha cursor state
V_STAT_0 = -0x006
* vertical resolution
V_REZ_VT = -0x004


USER_TIM = -0x42
NEXT_TIM = -0x3e
USER_BUT = -0x3a
USER_CUR = -0x36
USER_MOT = -0x32

* bit numbers for vt52 cursor state V_STAT_0
* blink enabled
CURSOR_BL           = 0
* currently visible
CURSOR_STATE        = 1
* cursor enabled (unused)
CURSOR_ONOFF        = 2
* wrap at eol
CURSOR_WRAP         = 3
* reverse video
CURSOR_INVERSE      = 4
* cursor position was saved
CURSOR_SAVED        = 5
