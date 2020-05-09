#ifndef SETJMP_H
#define SETJMP_H

typedef long jmp_buf[14]; /* retaddr, 12 regs, sigmask */

#define setjmp _mint_setjmp
#define longjmp _mint_longjmp

extern int   CDECL setjmp  (jmp_buf);
extern void  CDECL longjmp (jmp_buf, int);

#endif /* SETJMP_H */
