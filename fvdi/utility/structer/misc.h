#ifndef _MISC_H
#define _MISC_H
/*
 * Miscellaneous declarations
 *
 * Copyright 1997-2002, Johan Klockars
 * This software is licensed under the GNU General Public License.
 * Please, see LICENSE.TXT for further information.
 */

#include "hash.h"

typedef NTentry *Identifier;
typedef Identifier Name;
typedef long Numeral;
typedef char *String;

typedef enum _Type {_Char, _Short, _Int, _Long, _Struct, _Union,
                    _Structdef, _Uniondef, _Typedef, _Pointer, _Void} Type;

typedef enum _Listtype {_Deflist} Listtype;

#endif
