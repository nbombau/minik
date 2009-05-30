#include "../include/defs.h"
#include "../include/kc.h"
#include "../include/paging.h"
#include "../include/string.h"

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
  /*printf("mem antes de malloc\n");
  for(i = 0; i < 6; i = i+5)
  {
    printf( "%d * %d  *  %d  *  %d  * %d\n", mem[i],mem[i+1],mem[i+2],mem[i+3], mem[i+4]);
  }*/

    for(i = 0; i < MAX_PAGES; i++)
    {
        if(mem[i] == -1)
        {
            resp = (void *)(FIRST_USER_PAGE + i * PAGE_SIZE);
            mem[i] = proc->pid;
            /*printf("mem despues de malloc\n");
            for(i = 0; i <6; i = i+5)
            {
              printf( "%d * %d  *  %d  *  %d  * %d\n", mem[i],mem[i+1],mem[i+2],mem[i+3], mem[i+4]);
            }*/
            printf("stack malloc= ***%d*** -   dir=***%d***\n", (int)resp + PAGE_SIZE -1,(int)resp);
            return resp;
        }
    }

    return resp;
}


void labEBP(int * in, int * out, int offset, int old_ind)
{

    if(*out == 0)
    {
	printf("ebp final   %d\n", out );
        *in = *out;
        return;
    }

    *in = (*out % 4096)+ offset + (4096 * ((((*out)/4096) - 1024) - old_ind +1));

    labEBP((int*)*in, (int*)*out, offset, old_ind);
    return ;
}


void *
KRealloc(proceso_t * proc, int cantPaginas)
{
  void * resp = 0x0;
  int i,j,k,salgo=0,pos=-1, libere= FALSE;
  int marca,nuevoESP;
  /*printf("mem antes de realloc\n");
  for(i = 0; i < 6; i = i+5)
  {
    printf( "%d * %d  *  %d  *  %d  * %d\n", mem[i],mem[i+1],mem[i+2],mem[i+3], mem[i+4]);
  }*/

    /* primero buscamos las paginas del proceso
    */

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
   // if(pos == -1)
     //   return 0x0;
   /*levantar nuevas*/
   //habilitarPagina(proc);
    resp = (void *)(FIRST_USER_PAGE + pos * PAGE_SIZE);
    
    for(i=0;i<cantPaginas;i++)
        mem[pos+i] = proc->pid;
    
   // 
    //deshabilitarPagina(proc);
    /*printf("mem antes de free \n");
    for(i = 0; i < 6; i = i+5)
    {
      printf( "%d * %d  *  %d  *  %d  * %d\n", mem[i],mem[i+1],mem[i+2],mem[i+3], mem[i+4]);
    }*/
    
    KFree(marca, (int)(proc->stacksize/PAGE_SIZE));

    //memcpy(resp+cantPaginas*PAGE_SIZE-1-proc->stacksize,(void *)proc->stackstart-proc->stacksize,proc->stacksize);
    printf("\n%d - %d - %d\n",resp+cantPaginas*PAGE_SIZE-proc->stacksize,(void *)proc->stackstart-(cantPaginas-1)*PAGE_SIZE,(cantPaginas-1)*PAGE_SIZE);
    memcpy(resp+cantPaginas*PAGE_SIZE-proc->stacksize,(void *)proc->stackstart-(cantPaginas-1)*PAGE_SIZE+1,(cantPaginas-1)*PAGE_SIZE);

    proc->stackstart =(int) resp + cantPaginas* PAGE_SIZE -1;
    printf("ESP Antes: %d - Contenido: %d\n",proc->ESP,*((int *)proc->ESP));
    nuevoESP = (proc->ESP % PAGE_SIZE) + (int)resp+PAGE_SIZE;
    printf("ESP Despues: %d - Contenido: %d\n",proc->ESP,*((int *)proc->ESP));
    proc->stacksize = cantPaginas*PAGE_SIZE;

    ((STACK_FRAME *) nuevoESP)->EBP = (((STACK_FRAME *)(proc->ESP))->EBP % PAGE_SIZE) + (int)resp+PAGE_SIZE;
    
    
    /*((STACK_FRAME *) proc->ESP)->EBP = (((STACK_FRAME *)(proc->ESP))->EBP % 4096)+ (int)proc->stackstart;*/

    printf("EBP viejo %d - EBP nuevo %d\n",((STACK_FRAME *)(proc->ESP))->EBP,((STACK_FRAME *) nuevoESP)->EBP);
    labEBP( (int *)(((STACK_FRAME *) nuevoESP)->EBP) , (int *)(((STACK_FRAME *) proc->ESP)->EBP), (int)resp, marca );
    //_debug();
    proc->ESP=nuevoESP;

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

    /*printf("mem despues de free\n");
    for(i = 0; i < 6; i = i+5)
    {
      printf( "%d * %d  *  %d  *  %d  * %d\n", mem[i],mem[i+1],mem[i+2],mem[i+3], mem[i+4]);
    }*/
    printf("stack realloc= ***%d*** - dir=***%d*** - ESP: %d", (int)proc->stackstart, resp,proc->ESP);
   // _debug();
    printf("\nREALLOQUIE!\n");

    return resp+cantPaginas*PAGE_SIZE-1;
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
