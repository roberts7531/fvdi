/*
 * fVDI Calamus declarations
 *
 * Copyright 2004, Standa Opichal
 * This software is licensed under the GNU General Public License.
 * Please, see LICENSE.TXT for further information.
 */

#include "fvdi.h"


struct DCSD_BLITARGS {
    short handle;     /* Virtual screen workstation handle */
    short mode;       /* vro_cpyfm() mode */
    signed long x;    /* X position in pixels */
    signed long y;    /* Y position in pixels */
    unsigned long w;  /* Width in pixels */
    unsigned long h;  /* Height in pixels */
};

struct DCSD_cookie {
	short version;
	void  CDECL (*init)(void);
	void  CDECL (*exit)(void);
	long  CDECL (*active)(void);
	void* CDECL (*getbase)(void);
	void  CDECL (*gettlt)(unsigned char tlt[256]);
	void  CDECL (*blit_from_screen)(struct DCSD_BLITARGS *args);
	void  CDECL (*blit_to_screen)(struct DCSD_BLITARGS *args);
	long  CDECL (*custom)(long par);      /* Customer function */
};


void calamus_initialize_cookie(struct DCSD_cookie *cookie, short version);
void *CDECL dcsd_getbase(void);
void CDECL dcsd_gettlt(unsigned char tlt[256]);
void CDECL dcsd_blit_from_screen(struct DCSD_BLITARGS *args);
void CDECL dcsd_blit_to_screen(struct DCSD_BLITARGS *args);
long CDECL dcsd_active(void);
