#include "../include/defs.h"
#include "../include/kc.h"
#include "../include/paging.h"
#include "../include/string.h"

extern int veces;
extern int pidActual;
extern int maxmem;
extern int mem[MAX_PAGES];
extern unsigned long *page_table2;

void KFreeAux(int nPagina, int cantPaginas);

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


void ActualizaEBP(int * in, int * out, int offset, int old_ind)
{

    if(*out == 0)
    {
        *in = *out;
        return;
    }

    *in = (*out % 4096)+ offset + (4096 * ((((*out)/4096) - 1024) - old_ind +1));

    ActualizaEBP((int*)*in, (int*)*out, offset, old_ind);
    return ;
}

int prueba=0;
void *
KRealloc(proceso_t * proc, int cantPaginas)
{
    void * resp = 0x0,*ret=0x0;
    int i,j,k,salgo=0,pos=-1, libere= FALSE;
    int marca,nuevoESP;

    for(i = 0; i < MAX_PAGES && !libere; i++)
    {
        if(mem[i] == proc->pid)
        {
            marca = i;
            libere = TRUE;
        }
    }
    /*Busca cantPaginas contiguas libres para poder realocar el stack del proceso.*/
    for(j=0;j<(MAX_PAGES - cantPaginas) && pos==-1;j++)
    {
        for(k=0,salgo=0; k<cantPaginas && !salgo ;k++)
        {
            if(mem[j+k]!=-1)
                salgo=1;
        }
        if(!salgo)
            pos=j;
    }
    if(pos==-1)
	return 0x0;

    resp = (void *)(FIRST_USER_PAGE + pos * PAGE_SIZE);
    
    for(i=0;i<cantPaginas;i++)
        mem[pos+i] = proc->pid;
    /*Habilita todas las paginas. Tanto las viejas como las recien asignadas.*/
    HabilitarPaginaNuevo(proc);
    
    ret=memcpy(resp+cantPaginas*PAGE_SIZE-proc->stacksize,(void *)proc->stackstart-(cantPaginas-1)*PAGE_SIZE+1,(cantPaginas-1)*PAGE_SIZE);
    if(ret==0x0)
	return 0x0;
    
    /*Actualizo valores correspondientes al stack y actualizo los valores que que habian sido pusheados al stack.*/
    proc->stackstart =(int) resp + cantPaginas* PAGE_SIZE -1;

    nuevoESP = (proc->ESP % PAGE_SIZE) + (int)resp+PAGE_SIZE;

    ((STACK_FRAME *) nuevoESP)->EBP = (((STACK_FRAME *)(proc->ESP))->EBP % PAGE_SIZE) + (int)resp+PAGE_SIZE;
    
    ActualizaEBP( (int *)(((STACK_FRAME *) nuevoESP)->EBP) , (int *)(((STACK_FRAME *) proc->ESP)->EBP), (int)resp, marca );
    proc->ESP=nuevoESP;

    prueba++;
    

    _Cli();
    KFreeAux(marca, (int)(proc->stacksize/PAGE_SIZE));
    proc->stacksize = cantPaginas*PAGE_SIZE;
    HabilitarPaginaNuevo(proc);

    return resp+cantPaginas*PAGE_SIZE-1;
}

/*Marca las paginas como libres cuando un proceso deja de usarlas.*/
void
KFreeAux(int nPagina, int cantPaginas)
{
    int i;
    for(i = 0; i < cantPaginas; i++)
    {
        mem[nPagina+i] = -1;
    }
}

/*Libera la memoria alocada por un proceso*/
void
KFree(proceso_t * proc)
{
    int i;
    for(i=0;i<MAX_PAGES;i++)
    {
	if(mem[i]==proc->pid)
	{
	    DeshabilitarPaginas(proc);
	    mem[i]=-1;
	}
    }
}