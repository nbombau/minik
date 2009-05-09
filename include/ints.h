/***********************************************************************************
 * Ints.h
 *
 * Handlers de interrupciones
 *
 ***********************************************************************************/


#ifndef INTS_H_
#define INTS_H_

int int_09_LAT(unsigned char code);
int int_09_US(unsigned char code);
void int_80w(FileDesc fd, const void * buff, int size);
void int_80r(FileDesc fd, void * buff, int size);
char read_key(void);
void send_eoi(void);

#endif /*INTS_H_*/
