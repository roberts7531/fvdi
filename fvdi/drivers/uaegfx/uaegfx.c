/*
 * uaegfx.c - General functions
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

#include "uaegfx.h"
#include "relocate.h"

extern Access *access;

void uaegfx_puts(const char* message)
{
	access->funcs.puts("UAEGFX: ");
	access->funcs.puts(message);
	access->funcs.puts("\r\n");
}

static void wait(void)
{
	/* Unfortunately, fVDI can't recover from driver initialization failure.
	 * So we just wait forever. */
	for(;;);
}

void panic(const char* message)
{
	uaegfx_puts(message);
	wait();
}

void panic_help(const char* message)
{
	uaegfx_puts(message);
	access->funcs.puts(
		"Please open WinUAE Properties > Hardware > RTG board\r\n"
		"and select an RTG graphics card: UAE Zorro II or III.\r\n");
	wait();
}
