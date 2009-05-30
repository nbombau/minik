#include <stdarg.h>
#include "../include/stdio.h"

static void ImprimirString(const char * string, int * qttyPrinted);
static void ImprimirEntero(int num, int * qttyPrinted);
static void ImprimirCaracter(char c);

int
printf(const char * format, ...)
{
    va_list ap;
    const char * p, * sval;
    int ival, pchars = 0;
    //_Cli();
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
    //_Sti();
    return pchars;
}

static void
ImprimirCaracter(char c)
{
	writeWrapper(&c,1);
}

static void
ImprimirString(const char * string, int * qttyPrinted)
{
    if (string == 0x0)
        return;

    for (string ; *string ; string++, *qttyPrinted++)
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
        *qttyPrinted++;
    }

    if ( (num / 10) > 0 )
        ImprimirEntero(num/10, qttyPrinted);

    putchar(num % 10 + '0');
    *qttyPrinted++;

    return;
}

