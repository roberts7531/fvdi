void *memmove(void *dest, const void *src, size_t n)
{
    const char *s1;
    char *d1;

    if (((unsigned long) dest >= (unsigned long) src + n) || ((unsigned long) dest + n <= (unsigned long) src))
        return memcpy(dest, src, n);

    if ((unsigned long) dest < (unsigned long) src)
    {
        s1 = (const char *) src;
        d1 = (char *) dest;
        for (n--; (long) n >= 0; n--)
            *d1++ = *s1++;
    } else
    {
        s1 = (const char *) src + n;
        d1 = (char *) dest + n;
        for (n--; (long) n >= 0; n--)
            *(--d1) = *(--s1);
    }

    return dest;
}


