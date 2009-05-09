
#include "../include/kc.h"
/* Apartir de 8 mb */
int nextfree = 8192 * 1024 ;

int nextshmem = 0x0;

extern int pidActual;
extern int maxmem;

int initshmem = 0;

void *
Malloc (int size) {
    void *temp = (void *) nextfree;

    /* alinear a 4k */
    nextfree = nextfree + 2 * size + 4096 - (size % 4096);

    /* si ya era multiplo de 4k ocupe una pagina de mas */
    if (size % 4096 == 0)
        nextfree = nextfree - 4096;

    return temp;
}

void *
userMalloc (int size) {
    PROCESO *tmp;
    tmp = GetProcessByPid(pidActual);
    void *temp = 0x0;

    if ((tmp->nextfree + size) < (tmp->heapstart + tmp->heapsize)) {
        temp = (void *) tmp->nextfree;
        tmp->nextfree += size;
    }

    return temp;
}

