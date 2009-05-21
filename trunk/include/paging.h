#ifndef __PAGING__
#define __PAGING__

#include "../include/defs.h"

void InicializarMemUsuario(void);

void InitPaging(void);

void habilitarPagina(proceso_t * proc);

void deshabilitarPagina(proceso_t * proc);

#endif
