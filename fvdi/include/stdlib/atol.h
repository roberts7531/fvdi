long DRIVER_EXPORT atol(const char *text)
{
    long n;
    int minus, base, ch;

    while (isspace(*text))
        text++;

    minus = 0;
    if (*text == '-')
    {
        minus = 1;
        text++;
    }
    base = 10;
    if (*text == '$')
    {
        base = 16;
        text++;
    } else if (*text == '%')
    {
        base = 2;
        text++;
    }

    n = 0;
    while ((ch = check_base(*text++, base)) >= 0)
        n = n * base + ch;

    if (minus)
        n = -n;

    return n;
}
