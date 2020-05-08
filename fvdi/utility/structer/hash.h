#ifndef _HASH_H
#define _HASH_H
/*
 * Hashing declarations
 *
 * Copyright 1997-2002, Johan Klockars
 * This software is licensed under the GNU General Public License.
 * Please, see LICENSE.TXT for further information.
 */

#define NHASH 255

typedef struct _Scopeitem *Scopeitem;

struct _NTentry {
  int len;
  char *string;
  struct _NTentry*link;
} ;

typedef struct _NTentry NTentry;

extern void init_hash(int);
extern NTentry *new_string(const char *, int);
extern void hash_stat(int *, int *, int *);

#endif
