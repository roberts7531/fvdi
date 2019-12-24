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
#include "utility.h"
#include "fvdi.h"

/*
 * Structure definitions
 */
typedef struct _Prgheader
{
   short magic;
   long  tsize;
   long  dsize;
   long  bsize;
   long  ssize;
   long  reserved;
   long  flags;
   short relocflag;
} Prgheader;

typedef struct _Funcs
{
    void (*copymem)(const void *s, void *d, long n);
    const char * (*next_line)(const char *ptr);
    const char * (*skip_space)(const char *ptr);
    const char * (*get_token)(const char *ptr, char *buf, long n);
    long (*equal)(const char *str1, const char *str2);
    long (*length)(const char *text);
    void (*copy)(const char *src, char *dest);
    void (*cat)(const char *src, char *dest);
    long (*numeric)(long ch);
    long (*atol)(const char *text);
    void (*error)(const char *text1, const char *text2);
    void * (*malloc)(long size, long type);                    /* Uses Mxalloc if possible */
    long (*free)(void *addr);
    long (*puts)(const char *text);
    void (*ltoa)(char *buf, long n, unsigned long base);
    long (*get_cookie)(const char *cname, long super);
    long (*set_cookie)(const char *cname, long value);
    long (*fixup_font)(Fontheader *font, char *buffer, long flip);
    long (*unpack_font)(Fontheader *header, long format);
    long (*insert_font)(Fontheader **first_font, Fontheader *new_font);
    long (*get_size)(const char *name);
    void * (*allocate_block)(size_t size);
    void (*free_block)(void *address);
    void (*cache_flush)(void);
    long (*misc)(long func, long par, const char *token);
    long (*event)(long id_type, long data);
} Funcs;

typedef struct _Vars
{
    long *version;
    char *name;
} Vars;

typedef struct _Access
{
    Funcs funcs;
    Vars vars;
} Access;

typedef struct _Locator
{
    char magic[10];
    short version;
    long  (*init)(Access *, Driver *, Virtual *, char *);
} Locator;

#endif /* RELOCATE_H */
