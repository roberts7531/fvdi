#include "libkern.h"

/* From fvdi/engine/utility.c */
void *DRIVER_EXPORT malloc(size_t size);
void *realloc(void *addr, size_t new_size);
void DRIVER_EXPORT free(void *addr);

long labs(long j);
char *getenv(const char *s);

void qsort(void *base, long nmemb, long size, int (*compar) (const void *, const void *));
