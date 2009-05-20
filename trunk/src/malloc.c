#include "../include/defs.h"
#include "../include/kc.h"
#include "../include/paging.h"

extern int veces;
extern int pidActual;
extern int maxmem;
extern int mem[MAX_PAGES];


void *
KMalloc (proceso_t * proc) {

    void * resp = 0x0;
    int i;
    for(i = 0; i < MAX_PAGES; i++)
    {
        if(mem[i] == -1)
        {	
	    /*resp=(void*)(8 * 1024 * 1024 + 4096*i);*/
            resp = (void *)(FIRST_USER_PAGE + i * PAGE_SIZE-7);
            mem[i] = proc->pid;
            return resp;
        }
    }
    return resp;
}


void *
KRealloc(proceso_t * proc, int cantPaginas)
{

    /* primero buscamos las paginas del proceso
    */
    void * resp = 0x0;
    int i,j,k,salgo=0,pos=-1, libere= FALSE;
    int marca;
    for(i = 0; i < MAX_PAGES && !libere; i++)
    {
        if(mem[i] == proc->pid)
        {
            marca = i;
            libere = TRUE;
        }
    }

    for(j=0;j<(MAX_PAGES - cantPaginas) && pos==-1;j++)
    {
        for(k=0,salgo=0; /*mem[j]==-1 &&*/ k<cantPaginas && !salgo ;k++)
        {
            if(mem[j+k]!=-1)
                salgo=1;
        }
        if(!salgo)
            pos=j;
    }
    if(pos == -1)
        return 0x0;
   /*levantar nuevas*/
    resp = (void *)(FIRST_USER_PAGE + pos * PAGE_SIZE);
    for(i=0;i<cantPaginas-1;i++)
        mem[pos+i] = proc->pid;
    memcpy(resp+cantPaginas*PAGE_SIZE-proc->stacksize,proc->stackstart,proc->stacksize);
    
    KFree(marca, (int)(proc->stacksize/PAGE_SIZE));
    /*bajar paginas viejas*/
    /*bajar dichas paginas*/


    

    printf("\nREALLOQUIE!\n");

    return resp;
}

void
KFree(int nPagina, int cantPaginas)
{
    int i;
    for(i = 0; i < cantPaginas; i++)
    {
        mem[nPagina+i] = -1;
    }
}
