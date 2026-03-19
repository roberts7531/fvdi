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

#include "relocate.h"
#include "jox030r3.h"


uint8_t m_temp[128];

/* External data and functions */
extern Driver *me;
extern long CDECL c_xpand_area(Virtual *vwk, MFDB *src, long src_x, long src_y, MFDB *dst, long dst_x, long dst_y, long w, long h, long operation, long colour);
extern long CDECL c_blit_area(Virtual *vwk, MFDB *src, long src_x, long src_y, MFDB *dst, long dst_x, long dst_y, long w, long h, long operation);



extern Access *access;






long CDECL
c_mouse_draw(Workstation *wk, long x, long y, Mouse *mouse)
{
	
	if ((long)mouse > 7) /* Set new mouse cursor shape */
	{
		
		//cursor should be updated now, but for now I have only a single hardcoded cursor
		loadSprite(mouse->mask);
		return 0;
	}
	else
	{
				
		
		switch ((long)mouse)
		{
		case 0: /* Move visible */
		case 4: /* Move visible forced (wk_mouse_forced) */
			//show_mouse( (short)x, (short)y);
			setSpritePosition(x,y);
			break;

		case 1: /* Move hidden */
		case 5: /* Move hidden forced (wk_mouse_forced) */
			//show_mouse(vwk, (short)x, (short)y);
			//hide_mouse();
			setSpritePosition(x,y);

			break;

		case 2: /* Hide */
			//hide_mouse();
			setSpritePosition(x,y);

			break;

		case 3: /* Show */
			
			setSpritePosition(x,y);
//show_mouse( (short)x, (short)y);
			break;
		}

		return 0;
	}

	return 0;
}
