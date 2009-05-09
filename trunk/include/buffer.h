/***********************************************************************************
 * Buffer.h
 *
 * Cola utilizada por el driver controlador del teclado, para almacenar los asciis
 * que le llegan a traves de scandcodes por el teclado.
 *
 ***********************************************************************************/

#ifndef BUFFER_H
#define BUFFER_H

#define MAlsX_CANT 40
#define BUFFER_ARRAY_SIZE (MAX_CANT+1)
#define EOF -1
#define BUFFER_FULL -2

int AddToBuffer(unsigned char c);

unsigned char GetFromBuffer(void);

int BufferIsFull(void);

int BufferIsEmpty(void);

#endif
