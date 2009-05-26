#include "../include/defs.h"
#include "../include/kc.h"
#include "../include/paging.h"

extern int veces;
extern int pidActual;
extern int maxmem;
extern int mem[MAX_PAGES];

/*Asigna a un proceso una pagina en memoria para que pueda almacenar su stack.
  maxmem permite saber que pagina de memoria ( a partir de lo 8MB, que son las
  asignadas a los procesos usuario ) corresponde a cada proceso. Se marca cada
  pagina con el pid del proceso dueño.*/
void *
KMalloc (proceso_t * proc) {

    void * resp = 0x0;
    int i;
    for(i = 0; i < MAX_PAGES; i++)
    {
        if(mem[i] == -1)
        {
            resp = (void *)(FIRST_USER_PAGE + i * PAGE_SIZE);
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
   //habilitarPagina(proc);
    resp = (void *)(FIRST_USER_PAGE + pos * PAGE_SIZE);
    for(i=0;i<cantPaginas-1;i++)
        mem[pos+i] = proc->pid;
    memcpy(resp+cantPaginas*PAGE_SIZE-proc->stacksize,proc->stackstart,proc->stacksize);
    //deshabilitarPagina(proc);
    KFree(marca, (int)(proc->stacksize/PAGE_SIZE));
    
    /*bajar paginas viejas*/
    /*i=0;
    while(i<pos&&mem[i]!=-1)
    {
	if(mem[i]==proc->pid)
	    deshabilitarPagina(proc);
	i++;
    }*/
    
    /*bajar paginas recien creadas*/
    /*i=pos;
    while(i<MAX_PAGES && mem[i]!=-1)
    {
	if(mem[i]==proc->pid)
	    deshabilitarPagina(proc);
	i++;
    }*/

    

    printf("\nREALLOQUIE!\n");

    return resp;
}

/*Setea las paginas como libres. Cuando se solicite malloc podra otorgar estas paginas.*/
void
KFree(int nPagina, int cantPaginas)
{
    int i;
    for(i = 0; i < cantPaginas; i++)
    {
        mem[nPagina+i] = -1;
    }
}
