#ifndef __PAGING__
#define __PAGING__

#include "../include/defs.h"
#include "../include/kc.h"

void InicializarMemUsuario(void);

void InitPaging(void);

void HabilitarPaginaNuevo(proceso_t * proc);

void DeshabilitarPaginaNuevo(proceso_t * proc);

void HabilitarPaginas(proceso_t * proc);

void DeshabilitarPaginas(proceso_t * proc);

#endif
