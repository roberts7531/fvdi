/*
 * uae_mous.c - Mouse cursor functions
 * This is part of the WinUAE RTG driver for fVDI
 *
 * Copyright (C) 2017 Vincent Riviere
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/*#define ENABLE_KDEBUG*/

#include "fvdi.h"
#include <stdint.h>
#include "v99x8.h"
#include "relocate.h"



uint8_t m_temp[128];

/* External data and functions */
extern Driver *me;
extern long CDECL c_xpand_area(Virtual *vwk, MFDB *src, long src_x, long src_y, MFDB *dst, long dst_x, long dst_y, long w, long h, long operation, long colour);
extern long CDECL c_blit_area(Virtual *vwk, MFDB *src, long src_x, long src_y, MFDB *dst, long dst_x, long dst_y, long w, long h, long operation);



extern Access *access;




static void hide_mouse()
{
	v99x8_mode_r8_write(V99X8_MODE_R8_VR | V99X8_MODE_R8_TP |V99X8_MODE_R8_SPD);
}

static void show_mouse( short x, short y)
{
	v99x8_mode_r8_write(V99X8_MODE_R8_VR | V99X8_MODE_R8_TP);
	v99x8_sm2_sprite_attribute_write(0xFA00, 0, y, x/2, 0);
}

long CDECL
c_mouse_draw(Workstation *wk, long x, long y, Mouse *mouse)
{
	
	if ((long)mouse > 7) /* Set new mouse cursor shape */
	{
		//cursor should be updated now, but for now I have only a single hardcoded cursor

		return 0;
	}
	else
	{
			
		
		
		switch ((long)mouse)
		{
		case 0: /* Move visible */
		case 4: /* Move visible forced (wk_mouse_forced) */
			show_mouse( (short)x, (short)y);
			break;

		case 1: /* Move hidden */
		case 5: /* Move hidden forced (wk_mouse_forced) */
			//show_mouse(vwk, (short)x, (short)y);
			hide_mouse();
			break;

		case 2: /* Hide */
			hide_mouse();
			break;

		case 3: /* Show */
			show_mouse( (short)x, (short)y);
			break;
		}

		return 0;
	}

	return 0;
}
