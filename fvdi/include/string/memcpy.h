void DRIVER_EXPORT copymem(const void *s, void *d, long n)
{
    char *src;
    char *dest;

    src = (char *) s;
    dest = (char *) d;
    for (n = n - 1; n >= 0; n--)
        *dest++ = *src++;
}


void copymem_aligned(const void *s, void *d, long n)
{
    long *src, *dest, n4;

    src = (long *) s;
    dest = (long *) d;
    for (n4 = (n >> 2) - 1; n4 >= 0; n4--)
        *dest++ = *src++;

    if (n & 3)
    {
        char *s1 = (char *) src;
        char *d1 = (char *) dest;

        switch ((short) n & 3)
        {
        case 3:
            *d1++ = *s1++;
            /* fall through */
        case 2:
            *d1++ = *s1++;
            /* fall through */
        case 1:
            *d1++ = *s1++;
            break;
        }
    }
}


void *memcpy(void *_dest, const void *_src, size_t n)
{
    char *dest = (char *) _dest;
    const char *src = (const char *) _src;

    if (n > 3)
    {
        if (!((short)(long) dest & 1))
        {
            if (!((short)(long) src & 1))
            {
                copymem_aligned(src, dest, n);
                return dest;
            }
        } else if ((short)(long) src & 1)
        {
            *dest++ = *src++;
            copymem_aligned(src, dest, n - 1);
            return dest - 1;
        }
    }

    copymem(src, dest, n);

    return dest;
}
