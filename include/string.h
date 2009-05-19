#ifndef __STRING_H
#define __STRING_H

#define NULL 0x0

void    strncpy(char *dst, char *src, int num);
void    strncat(char *dst, char *src, int num);
int     strncmp(char *s1, char *s2, int num);
int atoi(char * string);
int getLine(char * buffer, int cant);
void    itoa( int num,char* number);
void * memcpy(void * dest, const void * src, unsigned int n);
void * memset(void * s, int c, unsigned int n);
#endif
