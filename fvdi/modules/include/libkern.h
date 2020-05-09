# ifndef _libkern_h
# define _libkern_h

# include <stdarg.h>
# include <sys/types.h>

typedef unsigned char uchar;

#ifndef DRIVER_EXPORT
#  define DRIVER_EXPORT CDECL
#endif
#ifndef CDECL
#ifdef __PUREC__
#define CDECL cdecl
#else
#define CDECL
#endif
#endif

#ifndef USE_LIBKERN
int isdigit(int c);
int isxdigit(int c);
int isalnum(int c);
int isspace(int c);
#else
# define str(x)		_stringify (x)
# define _stringify(x)	#x


/*
 * kernel character classification and conversion
 */

extern uchar _mint_ctype[];

# define _CTc		0x01	/* control character */
# define _CTd		0x02	/* numeric digit */
# define _CTu		0x04	/* upper case */
# define _CTl		0x08	/* lower case */
# define _CTs		0x10	/* whitespace */
# define _CTp		0x20	/* punctuation */
# define _CTx		0x40	/* hexadecimal */

# define isalnum(c)	(  _mint_ctype[(uchar)(c)] & (_CTu|_CTl|_CTd))
# define isalpha(c)	(  _mint_ctype[(uchar)(c)] & (_CTu|_CTl))
# define isascii(c)	(!((c) & ~0x7f))
# define iscntrl(c)	(  _mint_ctype[(uchar)(c)] &  _CTc)
# define isdigit(c)	(  _mint_ctype[(uchar)(c)] &  _CTd)
# define isgraph(c)	(!(_mint_ctype[(uchar)(c)] & (_CTc|_CTs)) && (_mint_ctype[(uchar)(c)]))
# define islower(c)	(  _mint_ctype[(uchar)(c)] &  _CTl)
# define isprint(c)	(!(_mint_ctype[(uchar)(c)] &  _CTc)       && (_mint_ctype[(uchar)(c)]))
# define ispunct(c)	(  _mint_ctype[(uchar)(c)] &  _CTp)
# define isspace(c)	(  _mint_ctype[(uchar)(c)] &  _CTs)
# define isupper(c)	(  _mint_ctype[(uchar)(c)] &  _CTu)
# define isxdigit(c)	(  _mint_ctype[(uchar)(c)] &  _CTx)
# define iswhite(c)	(isspace (c))

# define isodigit(c)	((c) >= '0' && (c) <= '7')
# define iscymf(c)	(isalpha(c) || ((c) == '_'))
# define iscym(c)	(isalnum(c) || ((c) == '_'))

# define _toupper(c)	((c) ^ 0x20)
# define _tolower(c)	((c) ^ 0x20)
# define _toascii(c)	((c) & 0x7f)
# define _toint(c)	((c) <= '9' ? (c) - '0' : toupper (c) - 'A')

int	CDECL _mint_tolower	(int c);
int	CDECL _mint_toupper	(int c);

static inline int
_mint_toupper_inline (register int c)
{
	return (islower (c) ? _toupper (c) : c);
}

static inline int
_mint_tolower_inline (register int c)
{
	return (isupper (c) ? _tolower (c) : c);
}

# define _ctype			_mint_ctype
# define toupper		_mint_toupper
# define tolower		_mint_tolower
# define TOUPPER		_mint_toupper_inline
# define TOLOWER		_mint_tolower_inline
#endif

/*
 * kernel string functions
 */

long	CDECL _mint_atol(const char *s);
long	CDECL strtonumber	(const char *name, long *result, int neg, int zerolead);

size_t	CDECL _mint_strlen(const char *s);

long	CDECL _mint_strcmp(const char *str1, const char *str2);
long	CDECL _mint_strncmp(const char *str1, const char *str2, ulong len);

long	CDECL _mint_stricmp	(const char *str1, const char *str2);
long	CDECL _mint_strnicmp	(const char *str1, const char *str2, long len);

char *	CDECL _mint_strcpy(char *dst, const char *src);
char *	CDECL _mint_strncpy(char *dst, const char *src, size_t len);
void	CDECL _mint_strncpy_f	(char *dst, const char *src, long len);

char *	CDECL _mint_strlwr	(char *s);
char *	CDECL _mint_strupr	(char *s);
char *	CDECL _mint_strcat(char *dst, const char *src);
char *	CDECL _mint_strchr(const char *str, long which);
char *	CDECL _mint_strrchr(const char *str, long which);
char *	CDECL _mint_strrev	(char *s);

char *	CDECL _mint_strstr	(const char *s, const char *w);

long	CDECL _mint_strtol	(const char *nptr, char **endptr, long base);
ulong	CDECL _mint_strtoul	(const char *nptr, char **endptr, long base);

void *	CDECL _mint_memchr(const void *s, long search, ulong size);
long	CDECL _mint_memcmp(const void *s1, const void *s2, ulong size);

# define atol			_mint_atol
# define strlen			_mint_strlen
# define strcmp			_mint_strcmp
# define strncmp		_mint_strncmp
# define stricmp		_mint_stricmp
# define strnicmp		_mint_strnicmp
# define strcpy			_mint_strcpy
# define strncpy		_mint_strncpy
# define strncpy_f		_mint_strncpy_f
# define strlwr			_mint_strlwr
# define strupr			_mint_strupr
# define strcat			_mint_strcat
# define strchr			_mint_strchr
# define strrchr		_mint_strrchr
# define strrev			_mint_strrev
# define strstr			_mint_strstr
# define strtol			_mint_strtol
# define strtoll		_mint_strtoll
# define strtoul		_mint_strtoul
# define strtoull		_mint_strtoull
# define memchr			_mint_memchr
# define memcmp			_mint_memcmp

/*
 * kernel block functions
 */

void *	CDECL memcpy(void *dst, const void *src, ulong nbytes);
void *	CDECL memset(void *dst, int ucharfill, ulong size);

#if 0
void	CDECL bcopy(const void *src, void *dst, ulong nbytes);
void	CDECL bzero(void *dst, ulong size);
#endif

# endif /* _libkern_h */
