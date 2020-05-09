#ifndef RELOCATE_H
#define RELOCATE_H
/*
 * fVDI driver->engine interface declarations, by Johan Klockars.
 *
 * Since it would be difficult to do without this file when
 * writing new device drivers, and to make it possible for
 * some such drivers to be commercial, this file is put in
 * the public domain. It's not copyrighted or under any sort
 * of license.
 */

#include "fvdi.h"
#include <string.h>
#include <stdarg.h>

/*
 * Structure definitions
 */
typedef struct _Prgheader {
   short magic;
   long  tsize;
   long  dsize;
   long  bsize;
   long  ssize;
   long  reserved;
   long  flags;
   short relocflag;
} Prgheader;

#ifndef DRIVER_EXPORT
#  define DRIVER_EXPORT CDECL
#endif

typedef struct _Funcs {
    void DRIVER_EXPORT (*copymem)(const void *s, void *d, long n);
    const char* DRIVER_EXPORT (*next_line)(const char *ptr);
    const char* DRIVER_EXPORT (*skip_space)(const char *ptr);
    const char* DRIVER_EXPORT (*get_token)(const char *ptr, char *buf, long n);
    long DRIVER_EXPORT (*equal)(const char *str1, const char *str2);
    long DRIVER_EXPORT (*length)(const char *text);
    void DRIVER_EXPORT (*copy)(const char *src, char *dest);
    void DRIVER_EXPORT (*cat)(const char *src, char *dest);
    long DRIVER_EXPORT (*numeric)(long ch);
    long DRIVER_EXPORT (*atol)(const char *text);
    void DRIVER_EXPORT (*error)(const char *text1, const char *text2);
    void* DRIVER_EXPORT (*malloc)(long size, long type);            /* Uses Mxalloc if possible */
    void DRIVER_EXPORT (*free)(void *addr);
    long DRIVER_EXPORT (*puts)(const char *text);
    void DRIVER_EXPORT (*ltoa)(char *buf, long n, unsigned long base);
    long DRIVER_EXPORT (*get_cookie)(const char *cname, long super);
    long DRIVER_EXPORT (*set_cookie)(const char *cname, long value);
    long DRIVER_EXPORT (*fixup_font)(Fontheader *font, char *buffer, long flip);
    long DRIVER_EXPORT (*unpack_font)(Fontheader *header, long format);
    long DRIVER_EXPORT (*insert_font)(Fontheader **first_font, Fontheader *new_font);
    long DRIVER_EXPORT (*get_size)(const char *name);
    char* DRIVER_EXPORT (*allocate_block)(long size);
    void DRIVER_EXPORT (*free_block)(void *address);
    void DRIVER_EXPORT (*cache_flush)(void);
    long DRIVER_EXPORT (*misc)(long func, long par, const char *token);
    void DRIVER_EXPORT (*event)(long id_type, long data);
} Funcs;

typedef struct _Vars {
    long *version;
    char *name;
} Vars;

typedef struct _Access {
    Funcs funcs;
    Vars vars;
} Access;

typedef struct _Locator {
    char magic[10];
    short version;
    long  CDECL (*init)(Access *, Driver *, Virtual *, char *);
} Locator;

#endif /* RELOCATE_H */
