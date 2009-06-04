#include "../include/string.h"
#include "../include/kc.h"


void
strncpy (char *dst, char *src, int num)
{
    if (num != 0)
    {

        do {
            if ((*dst++ = *src++) == 0)
	    {
                while (--num != 0)
                    *dst++ = 0;
                break;
            }
        } while (--num != 0);
    }
}

int
strlen(char *s)
{
    int i = 0;

    while (*s++)
        i++;

    return i;
}

void
strncat (char *dst, char *src, int num)
{
    while (*dst++);

    --dst;

    while (*dst++ == *src++)
        if (--num < 0)
	{
            *--dst = '\0';
            break;
        }
}

int
strncmp (char *s1, char *s2, int num) {
    if (num == 0)
        return 0;

    do {
        if (*s1 != *s2++)
            return (*s1 - *(s2 - 1));
        if (*s1++ == 0)
            break;
    } while (--num != 0);

    return 0;
}


int
atoi (char *string)
{
    int digito, numero = 0, i = 0;
    char c;

    while ((c = string[i++]) != '\n' && (c != '\0'))
    {
        if (c >= '0' && c <= '9')
	{
            digito = c - '0';
        }
        else
            return -1;

        numero = (numero * 10) + digito;
    }
    return numero;

}

int
getLine (char *buffer, int cant)
{
    int i = 0;
 /*   char c;



    while ((c = getChar ()) != '\n' && i < cant)
        buffer[i++] = c;

    if (i >= cant) {
        //  CLN_BUF;
        return -1;
    }

    buffer[i] = 0;*/



    return i;
}

int
resto (int *num, int divisor)
{
    int resto, aux = *num;
    int i = 0;

    if (divisor < 0 || *num < 0 || *num - divisor < 0)
        return -1;

    do {

        aux = aux - divisor;

        if (aux >= 0)
            resto = aux;
        else
            break;

        i++;

    } while (1);

    *num = i;

    return resto;

}

char buffer[10];

void
itoa (int num, char *number)
{
    int i = 8, j, res, aux = num;

    if( number == NULL )
        number = buffer;

    for (j = 0; j < i; j++) 
    {
        number[j] = ' ';
    }


    for (i = i - 1; (res = resto (&aux, 10)) != -1; i--)
    {
        number[i] = res + '0';

    }

    number[i] = aux + '0';
    
    if(number == buffer )
	printf(buffer,10);
}





void * 
memcpy(void * dest, const void * src, unsigned int n)
{
    _Cli();
    int i;
    unsigned char * current;

    if (dest == 0x0 || src == 0x0 || n <= 0)
        return 0x0;

    for (i = 0 ; i < n ; i++)
    {
        /* Si hay memory overlap (dest interseccion src != vacio), falla la copia */
        if ( (unsigned char *)dest + i * sizeof(char) == ( (unsigned char *)src ) + i * sizeof(char) )
            return 0x0;

        /* Obtiene la direccion del byte a donde tiene que escribir */
        current = (unsigned char *)dest + i * sizeof(char);

        /* escribe sobre esa posicion de memoria */
        *current = *((unsigned char *)src + i * sizeof(char));
	
    }
    
	return dest;
}


void *
memset(void * s, int c, unsigned int n)
{
    int i;
    unsigned char * current;

    if (s == 0x0 || n <= 0)
        return 0x0;

    for (i = 0 ; i < n ; i++)
    {
        current = (unsigned char *)s + i * sizeof(unsigned char);
        *current = (unsigned char)c;
    }

    return s;
}

