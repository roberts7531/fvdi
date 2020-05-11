#ifndef GLOBALS_H
#define GLOBALS_H
/*
 * fVDI global variable declarations
 *
 * Copyright 2003, Johan Klockars
 * This software is licensed under the GNU General Public License.
 * Please, see LICENSE.TXT for further information.
 */

#include "relocate.h"

extern Virtual *handle[];
extern Virtual *default_virtual;
extern Virtual *non_fvdi_vwk;
extern Workstation *non_fvdi_wk;
extern Workstation *screen_wk;
extern List *driver_list;
extern short old_gdos;

extern short old_wk_handle;

extern short vbl_handler_installed;

extern long basepage;
extern short key_pressed;

extern List *driver_list;
extern long *pid;

extern void (*trap2_address)(void);
extern void (*vdi_address)(void);
extern void (*trap14_address)(void);
extern void (*lineA_address)(void);

void trap2_temp(void);
void trap14(void);
void lineA(void);
void vdi_dispatch(void);
void eddi_dispatch(void);
void init(void);
void data_start(void);
void bss_start(void);

extern void *bconout_address;
void bconout_stub(void);

extern long mint;
extern long magic;

extern void* dummy_vdi;

extern Function default_functions[];
extern void *default_opcode5[];
extern void *default_opcode11[];

extern short *pattern_ptrs[];
extern short solid[];

extern short width;
extern short height;

extern long sub_call;


extern Access real_access;
extern Access *access;

extern long *pid;

extern Workstation *screen_wk;
extern Virtual     *screen_vwk;

extern char *vdi_stack_top;
extern long vdi_stack_size;

/*
 * Option values
 */
extern short disabled;
extern short booted;
extern short fakeboot;
extern short oldmouse;
extern short debug;
extern short nvdifix;
extern short lineafix;
extern short bconout;
extern short bconout_redir;
extern short xbiosfix;
extern short singlebend;
extern short memlink;
extern short blocks;
extern long block_size;
extern long log_size;
extern short arc_split;
extern short arc_min;
extern short arc_max;
extern short debug_out;
extern short interactive;
extern short stand_alone;
extern short nvdi_cookie;
extern short speedo_cookie;
extern short calamus_cookie;
extern char silent[];
extern char silentx[];
extern unsigned short sizes[];
extern short size_count;
extern short old_malloc;
extern short fall_back;
extern short move_mouse;
extern short ext_malloc;
#ifdef FVDI_DEBUG 
extern short check_mem;
#endif
extern short bconout;
extern short file_cache_size;
extern short antialiasing;
extern char *debug_file;

extern long pid_addr;


/*
 * VDI call arrays
 */
extern short control[];
extern short int_in[];
extern short pts_in[];
extern short int_out[];
extern short pts_out[];

extern struct Super_data *super;

#endif
