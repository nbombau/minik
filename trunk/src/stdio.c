#include <stdarg.h>
#include "../include/stdio.h"
#include "../include/video.h"
static void ImprimirString(const char * string, int * qttyPrinted);
static void ImprimirEntero(int num, int * qttyPrinted);

int
printf(const char * format, ...)
{
    va_list ap;
    const char * p, * sval;
    int ival, pchars = 0;
    if (format == 0x0)
        return 0;

    va_start(ap, format);

    for (p = format; *p ; p++)
    {
        if (*p != '%')
        {
            putchar(*p);
            pchars++;
        }
        else
        {
            switch(*++p)
            {
                case 'd':
                        ival = va_arg(ap, int);
                        ImprimirEntero(ival, &pchars);
                        break;

                case 's':
                        sval = va_arg(ap, char *);
                        ImprimirString(sval, &pchars);
                        break;

                case 'c':
                        ival = va_arg(ap, int);
                        putchar(ival);
                        pchars++;
                        break;

                default:
                        putchar(*p);
                        pchars++;
            }
        }
    }
    va_end(ap);
    return pchars;
}


static void
ImprimirString(const char * string, int * qttyPrinted)
{
    if (string == 0x0)
        return;

    for ( ; *string ; string++, (*qttyPrinted)++)
        putchar(*string);

    return;
}


static void
ImprimirEntero(int num, int * qttyPrinted)
{
    if (num < 0)
    {
        putchar('-');
        num = -num;
        (*qttyPrinted)++;
    }

    if ( (num / 10) > 0 )
        ImprimirEntero(num/10, qttyPrinted);

    putchar(num % 10 + '0');
    (*qttyPrinted)++;

    return;
}

