/*
 * types.h - Type definitions and macros
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

#ifndef TYPES_H
#define TYPES_H

/*
 * The following macros have been borrowed from EmuTOS.
 * https://sourceforge.net/p/emutos/code/ci/master/tree/include/portab.h
 */

/* Convenience macros to test the versions of glibc and gcc.
   Use them like this:
   #if __GNUC_PREREQ (2,8)
   ... code requiring gcc 2.8 or later ...
   #endif
   Note - they won't work for gcc1 or glibc1, since the _MINOR macros
   were not defined then.  */
#if defined __GNUC__ && defined __GNUC_MINOR__
# define __GNUC_PREREQ(maj, min) \
	((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#else
# define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(3, 3)
# define MAY_ALIAS __attribute__ ((__may_alias__))
#else
# define MAY_ALIAS
#endif

#if __GNUC_PREREQ(3, 3)
# define __CLOBBER_RETURN(a)
#else
# define __CLOBBER_RETURN(a) a,
#endif

/* Types used by UAE */
typedef short uae_s16;
typedef unsigned char uae_u8;
typedef unsigned short uae_u16;
typedef unsigned long uae_u32;

/* Other types */
typedef short WORD;
typedef unsigned char UBYTE;
typedef unsigned short UWORD;
typedef unsigned long ULONG;
typedef signed char BYTE;
typedef int BOOL;

/* Strict aliasing counter-measures */
typedef short short_ALIAS MAY_ALIAS;

#define NULL ((void*)0)

#endif /* TYPES_H */
