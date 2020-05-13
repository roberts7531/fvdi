/* Mostly complete, but only simple things tested */
long DRIVER_EXPORT kvsprintf(char *str, const char *format, va_list args)
{
    int mode = 0;
    char *s, *text, *orig_s, ch;
    long val_l;
    int base = 10;
    int field = 0;
    int precision = 0;
    int opts = 0;

    s = orig_s = str;

#define OPTS_ZEROPAD   0x0001
#define OPTS_SIGNP     0x0002
#define OPTS_SIGN      0x0004
#define OPTS_JUSTLEFT  0x0008
#define OPTS_HEXPREFIX 0x0010
#define OPTS_UNSIGNED  0x0020

    while ((ch = *format++) != 0)
    {
        if (mode)
        {
            switch (ch)
            {
            case 's':
                text = va_arg(args, char *);
                while ((*str++ = *text++) != 0)
                    ;
                str--;
                mode = 0;
                break;

            case 'c':
                *str++ = va_arg(args, int); /* char promoted to int */
                mode = 0;
                break;

            case 'p':
                opts |= OPTS_HEXPREFIX;
                mode = 4;
                /* fall through */
            case 'x':
            case 'X':
                base = 16;
                if (opts & OPTS_HEXPREFIX)
                {
                    *str++ = '0';
                    *str++ = 'x';
                }
                /* fall through */
            case 'o':
                if (base == 10)
                {
                    base = 8;
                    if (opts & OPTS_HEXPREFIX)
                    {
                        *str++ = '0';
                    }
                }
                /* fall through */
            case 'u':
                opts |= OPTS_UNSIGNED;   /* Unsigned conversion */
                /* fall through */
            case 'd':
            case 'i':
                if (!(opts & 0x0100) && (opts & OPTS_ZEROPAD) && !(opts & OPTS_JUSTLEFT))
                {
                    precision = field;
                    field = 0;
                }
                switch (mode)
                {
                case 4:
                    val_l = va_arg(args, long);
                    break;
                case 3:
                    if (opts & OPTS_UNSIGNED)
                        val_l = (unsigned long) (unsigned char) va_arg(args, int);
                    else
                        val_l = (long) (signed char) va_arg(args, int);
                    break;
                case 2:
                    if (opts & OPTS_UNSIGNED)
                        val_l = (unsigned long) (unsigned short) va_arg(args, int);
                    else
                        val_l = (long) (signed short) va_arg(args, int);
                    break;
                case 1:
                    if (opts & OPTS_UNSIGNED)
                        val_l = va_arg(args, unsigned int);
                    else
                        val_l = va_arg(args, int);
                    break;
                default:
                    val_l = 0;
                    break;
                }
                if (!(opts & OPTS_UNSIGNED))
                {
                    if (val_l > 0)
                    {
                        if (opts & OPTS_SIGN)
                            *str++ = '+';
                        else if (opts & OPTS_SIGNP)
                            *str++ = ' ';
                    } else if (val_l < 0)
                    {
                        *str++ = '-';
                        val_l = -val_l;
                    }
                }
                if (val_l || !(opts & 0x0100) || (precision != 0))
                    ultoa(str, val_l, base);
                val_l = strlen(str);
                if (val_l < precision)
                {
                    memmove(str + precision - val_l, str, val_l + 1);
                    memset(str, '0', precision - val_l);
                }
                str += strlen(str);
                mode = 0;
                break;

            case '%':
                *str++ = '%';
                mode = 0;
                break;

            case 'h':
                opts |= 0x8000;
                if (mode == 1)
                    mode = 2;
                else if (mode == 2)
                    mode = 3;
                else
                    opts |= 0x4000;
                break;

            case 'z':
            case 't':
            case 'l':
                opts |= 0x8000;
                if (mode == 1)
                    mode = 4;
                else
                    opts |= 0x4000;
                break;

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if (mode == 1)
                {
                    ch -= '0';
                    if (!ch && !(opts & 0x8000))
                    {
                        if (!(opts & OPTS_ZEROPAD))
                            opts |= OPTS_ZEROPAD;   /* Zero padding */
                        else
                            opts |= 0x4000;
                    } else
                    {
                        opts |= 0x8000;
                        if (opts & 0x0100)
                        {
                            if (!(opts & 0x0200))
                            {
                                opts |= 0x0400;
                                precision = precision * 10 + ch;
                            } else
                                opts |= 0x4000;
                        } else if (!(opts & 0x0800))
                        {
                            opts |= 0x0400;
                            field = field * 10 + ch;
                        } else
                        {
                            opts |= 0x4000;
                        }
                    }
                } else
                {
                    opts |= 0x4000;
                }
                break;

            case '*':
                if (mode == 1)
                {
                    if (opts & 0x0100)
                    {
                        if (!(opts & 0x0600))
                        {
                            opts |= 0x0200;
                            precision = va_arg(args, int);

                            if (precision < 0)
                                precision = 0;
                        } else
                        {
                            opts |= 0x4000;
                        }
                    } else if (!(opts & 0x8c00))
                    {
                        opts |= 0x8800;
                        field = va_arg(args, int);

                        if (field < 0)
                        {
                            opts |= OPTS_JUSTLEFT;
                            field = -field;
                        }
                    } else
                    {
                        opts |= 0x4000;
                    }
                } else
                {
                    opts |= 0x4000;
                }
                break;

            case ' ':
                if (!(opts & (0x8000 | OPTS_SIGNP)))
                {
                    opts |= OPTS_SIGNP;     /* Space in front of positive numbers */
                } else
                {
                    opts |= 0x4000;
                }
                break;

            case '+':
                if (!(opts & (0x8000 | OPTS_SIGN)))
                {
                    opts |= OPTS_SIGN;      /* Sign in front of all numbers */
                } else
                {
                    opts |= 0x4000;
                }
                break;

            case '-':
                if (!(opts & (0x8000 | OPTS_JUSTLEFT)))
                {
                    opts |= OPTS_JUSTLEFT;  /* Left justified field */
                } else
                {
                    opts |= 0x4000;
                }
                break;

            case '#':
                if (!(opts & (0x8000 | OPTS_HEXPREFIX)))
                {
                    opts |= OPTS_HEXPREFIX; /* 0x/0 in front of hexadecimal/octal numbers */
                } else
                {
                    opts |= 0x4000;
                }
                break;

            case '.':
                if (!(opts & 0x0100) && (mode == 1))
                {
                    opts &= ~0x0400;
                    opts |= 0x8100;
                    precision = 0;
                } else
                {
                    opts |= 0x4000;
                }
                break;

            default:
                opts |= 0x4000;
                break;
            }

            if (opts & 0x4000)
            {
                *str++ = '%';
                *str++ = '?';
                mode = 0;
            }

            if ((mode == 0) && (field > str - s))
            {
                val_l = field - (str - s);
                if (opts & OPTS_JUSTLEFT)
                {
                    memset(str, ' ', val_l);
                } else
                {
                    memmove(s + val_l, s, str - s);
                    memset(s, ' ', val_l);
                }
                str += val_l;
            }
        } else if (ch == '%')
        {
            mode = 1;
            base = 10;
            opts = 0;
            field = 0;
            precision = 0;
            s = str;
        } else
        {
            *str++ = ch;
        }
    }
    *str = 0;

    return strlen(orig_s);
}


long DRIVER_EXPORT ksprintf(char *str, const char *format, ...)
{
    va_list args;
    long ret;

    va_start(args, format);
    ret = kvsprintf(str, format, args);
    va_end(args);
    return ret;
}


long DRIVER_EXPORT kprintf(const char *format, ...)
{
    va_list args;
    char buf[512];
    long ret;
    
    va_start(args, format);
    ret = kvsprintf(buf, format, args);
    va_end(args);
    access->funcs.puts(buf);
    return ret;
}
