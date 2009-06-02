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

int prueba=0;
void *
KRealloc(proceso_t * proc, int cantPaginas)
{
    void * resp = 0x0,*ret=0x0;
    int i,j,k,salgo=0,pos=-1, libere= FALSE;
    int marca,nuevoESP;
    clear_screen();


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
        for(k=0,salgo=0; k<cantPaginas && !salgo ;k++)
        {
            if(mem[j+k]!=-1)
                salgo=1;
        }
        if(!salgo && mem[j]==-1)
            pos=j;
    }

    resp = (void *)(FIRST_USER_PAGE + pos * PAGE_SIZE);
    
    for(i=0;i<cantPaginas;i++)
        mem[pos+i] = proc->pid;

    for(i = 0; i < 6; i = i+5)
    {
      printf( "%d * %d  *  %d  *  %d  * %d\n", mem[i],mem[i+1],mem[i+2],mem[i+3], mem[i+4]);
    }
    
    KFree(marca, (int)(proc->stacksize/PAGE_SIZE));

    printf("\n%d - %d - %d\n",resp+cantPaginas*PAGE_SIZE-proc->stacksize,(void *)proc->stackstart-(cantPaginas-1)*PAGE_SIZE,(cantPaginas-1)*PAGE_SIZE);
    
    ret=memcpy(resp+cantPaginas*PAGE_SIZE-proc->stacksize,(void *)proc->stackstart-(cantPaginas-1)*PAGE_SIZE+1,(cantPaginas-1)*PAGE_SIZE);
    Paginas(NULL,NULL);
    proc->stackstart =(int) resp + cantPaginas* PAGE_SIZE -1;
    printf("ESP Antes: %d - Contenido: %d\n",proc->ESP,*((int *)proc->ESP));
    nuevoESP = (proc->ESP % PAGE_SIZE) + (int)resp+PAGE_SIZE;
    printf("ESP Despues: %d - Contenido: %d\n",proc->ESP,*((int *)proc->ESP));
    proc->stacksize = cantPaginas*PAGE_SIZE;

    ((STACK_FRAME *) nuevoESP)->EBP = (((STACK_FRAME *)(proc->ESP))->EBP % PAGE_SIZE) + (int)resp+PAGE_SIZE;
    
    printf("EBP viejo %d - EBP nuevo %d\n",((STACK_FRAME *)(proc->ESP))->EBP,((STACK_FRAME *) nuevoESP)->EBP);
    labEBP( (int *)(((STACK_FRAME *) nuevoESP)->EBP) , (int *)(((STACK_FRAME *) proc->ESP)->EBP), (int)resp, marca );
    proc->ESP=nuevoESP;

    prueba++;
        
    printf("stack realloc= ***%d*** - dir=***%d*** - ESP: %d", (int)proc->stackstart, resp,proc->ESP);

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
