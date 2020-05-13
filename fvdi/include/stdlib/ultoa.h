void ultoa(char *buf, unsigned long un, unsigned long base)
{
    char *tmp, ch;

    tmp = buf;
    do
    {
        ch = un % base;
        un = un / base;
        if (ch <= 9)
            ch += '0';
        else
            ch += 'a' - 10;
        *tmp++ = ch;
    } while (un);
    *tmp = '\0';
    while (tmp > buf)
    {
        ch = *buf;
        *buf++ = *--tmp;
        *tmp = ch;
    }
}
