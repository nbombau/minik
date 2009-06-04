#include "defs.h"

/**********************************
*
*  Kernel
*
***********************************/

#ifndef _kernel_
#define _kernel_

#include "../include/defs.h"

#define OS_PID	0

int (*player)(void);

typedef int size_t;
typedef short int ssize_t;


/* __write
*
* Recibe como parametros:
* - File Descriptor
* - Buffer del source
* - Cantidad
*
**/
size_t __write(int fd, const void* buffer, size_t count);

/* __read
*
* Recibe como parametros:
* - File Descriptor
* - Buffer a donde escribir
* - Cantidad
*
**/
size_t __read(int fd, void* buffer, size_t count);

void loadkeysla(void);

void loadkeysus(void);

#endif
