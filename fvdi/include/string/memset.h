static void setmem(void *d, long v, long n)
{
    char *dest;

    dest = (char *) d;
    for (n = n - 1; n >= 0; n--)
        *dest++ = (char) v;
}


static void setmem_aligned(void *d, unsigned char c, long n)
{
    long *dest;
    long n4;
    unsigned long v;

    v = ((unsigned short) c << 8) | (unsigned short) c;
    v = (v << 16) | v;
    dest = (long *) d;
    for (n4 = (n >> 2) - 1; n4 >= 0; n4--)
        *dest++ = v;

    if (n & 3)
    {
        char *d1 = (char *) dest;

        switch ((short) n & 3)
        {
        case 3:
            *d1++ = (char) (v);
            /* fall through */
        case 2:
            *d1++ = (char) (v);
            /* fall through */
        case 1:
            *d1++ = (char) (v);
            break;
        }
    }
}


/* This function needs an 'int' parameter
 * to be compatible with gcc's built-in
 * version.
 * For module use, a separate version will
 * be needed since they can't be guaranteed
 * to have the same size for 'int'.
 */
void *memset(void *s, int c, size_t n)
{
    if ((n > 3) && !((long) s & 1))
        setmem_aligned(s, c, n);
    else
        setmem(s, c, n);

    return s;
}


