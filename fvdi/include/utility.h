#ifndef UTILITY_H
#define UTILITY_H

/*
 * fVDI utility function declarations
 *
 * Copyright 2003, Johan Klockars
 * This software is licensed under the GNU General Public License.
 * Please, see LICENSE.TXT for further information.
 */

long init_utility(void);

/*
 * Memory access
 */
long get_protected_l(long addr);
void set_protected_l(long addr, long value);
long get_l(long addr);
void set_l(long addr, long value);

/*
 * Cookie and XBRA access
 */
long DRIVER_EXPORT get_cookie(const char *cname, long super);
long DRIVER_EXPORT set_cookie(const char *name, long value);
long remove_xbra(long vector, const char *name);

/*
 * Memory pool allocation (from set of same sized blocks)
 */
long initialize_pool(long size, long n);
char *DRIVER_EXPORT allocate_block(long size);
void DRIVER_EXPORT free_block(void *addr);


/*
 * Memory/string operations
 */
void DRIVER_EXPORT copymem(const void *s, void *d, long n);
void copymem_aligned(const void *s, void *d, long n);
void DRIVER_EXPORT copy(const char *src, char *dest);
void DRIVER_EXPORT cat(const char *src, char *dest);
long DRIVER_EXPORT length(const char *text);
long DRIVER_EXPORT equal(const char *str1, const char *str2);

/*
 * Character numerics
 */
long DRIVER_EXPORT numeric(long ch);
int check_base(char ch, int base);
long DRIVER_EXPORT atol(const char *text);
void DRIVER_EXPORT ltoa(char *buf, long n, unsigned long base);
void ultoa(char *buf, unsigned long un, unsigned long base);
long str2long(const char *text);

/*
 * General memory allocation
 */
void *fmalloc(long size, long type);
void *malloc(size_t size);
void *realloc(void *addr, size_t new_size);
void *calloc(size_t nmemb, size_t size);
long free_size(void *addr);
void free(void *addr);
long free_all(void);
void allocate(long amount);
#ifdef FVDI_DEBUG 
void check_memory(void);
#endif

/*
 * Text output
 */
#ifdef FVDI_DEBUG
#include "relocate.h"
extern Access *access;
#define PUTS(x) access->funcs.puts(x)
#define PRINTF(x) kprintf x
#define KEY_WAIT(x) key_wait(x)
#else
#define PUTS(x)
#define PRINTF(x)
#define KEY_WAIT(x)
#endif

void DRIVER_EXPORT error(const char *text1, const char *text2);

long DRIVER_EXPORT kputs(const char *text);
long DRIVER_EXPORT kprintf(const char *format, ...) __attribute__((format(printf, 1, 2)));
long DRIVER_EXPORT ksprintf(char *buf, const char *format, ...) __attribute__((format(printf, 2, 3)));
long DRIVER_EXPORT kvsprintf(char *buf, const char *format, va_list args) __attribute__((format(printf, 2, 0)));

/*
 * Token handling
 */
const char *DRIVER_EXPORT next_line(const char *ptr);
const char *DRIVER_EXPORT skip_space(const char *ptr);
const char *skip_only_space(const char *ptr);
const char *DRIVER_EXPORT get_token(const char *ptr, char *buf, long n);

/*
 * Miscellaneous
 */
long DRIVER_EXPORT misc(long func, long par, const char *token);
void flip_words(void *addr, long n);
void flip_longs(void *addr, long n);
void initialize_palette(Virtual *vwk, long start, long n,
                        short colours[][3], Colour *palette);
void DRIVER_EXPORT cache_flush(void);
long DRIVER_EXPORT get_size(const char *name);

/*
 * VDI/AES
 */
short appl_init(void);
short appl_exit(void);
short graf_handle(void);
short call_v_opnwk(long handle, short *int_out, short *pts_out);
short scall_v_opnwk(long handle, short *int_out, short *pts_out);
short call_v_opnvwk(long handle, short *int_out, short *pts_out);
short call_v_clsvwk(long handle);
void vq_extnd(long handle, long info_flag, short *int_out, short *pts_out);
void vq_color(long handle, long colour, long flag, short *int_out);
void set_inout(short *int_in, short *pts_in, short *int_out, short *pts_out);
void vdi(long handle, long func, long pts, long ints);
void sub_vdi(long handle, long func, long pts, long ints);
void fvdi(long handle, long func, long pts, long ints);
long get_sub_call(void);
long vq_gdos(void);
short call_other(VDIpars *pars, long handle);

/*
 * Fonts
 */
long DRIVER_EXPORT fixup_font(Fontheader *font, char *buffer, long flip);
long DRIVER_EXPORT unpack_font(Fontheader *header, long format);
long DRIVER_EXPORT insert_font(Fontheader **first_font, Fontheader *new_font);
Fontheader *load_font(const char *name);

/*
 * Maths
 */
short isqrt(unsigned long x);
short Isin(unsigned short angle);
short Icos(short angle);

#define ABS(x)    (((x) >= 0) ? (x) : -(x))
#define MIN(x,y)  (((x) < (y)) ? (x) : (y))
#define MAX(x,y)  ((x) > (y) ? (x) : (y))

#endif
