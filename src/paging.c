#include "../include/paging.h"
int mem[MAX_PAGES];
unsigned long *page_directory = (unsigned long *) 0x9C000;
unsigned long *page_table1 = (unsigned long *) 0x9D000;
unsigned long *page_table2 = (unsigned long *) 0x9E000;


void
InicializarMemUsuario(void)
{
    int i;

    for(i = 0; i<MAX_PAGES; i++)
    {
	mem[i] = -1;
    }
}

void
InitPaging(void)
{
    unsigned long address = 0;
    unsigned int i;
    for(i=0; i<1024; i++)
    {
      page_table1[i] = address | 3;
      address = address + 4096;
    }

    for(i=0; i<1024; i++)
    {
      page_table2[i] = address & 0xFFFFFFFE;
      address = address + 4096;
    }
    
    page_directory[0] = (int) page_table1;
    page_directory[0] = page_directory[0] | 3;

    page_directory[1] = (int) page_table2;
    page_directory[1] = page_directory[1] | 3;

    for(i=2; i<1024; i++)
    {
      page_directory[i] = 0 | 2;
    }

    /*En el registro CR3 guardo cual es el directorio de paginas.
     *Seteo el bie correspondiente en CR0 para habilitar la paginacion.
    */
    write_cr3 (page_directory);
    write_cr0 ((unsigned long *)(read_cr0 () | 0x80000000));
}


void
HabilitarPaginaNuevo(proceso_t * proc)
{
    int i;
    for(i=0;i<MAX_PAGES-1;i++)
    {
	if(mem[i]==proc->pid)
	{
	    page_table2[i]=page_table2[i] | 3;
	}
    }
}


void
DeshabilitarPaginaNuevo(proceso_t * proc)
{
    int i;
    for(i=0;i<MAX_PAGES-1;i++)
    {
	if(mem[i]==proc->pid)
	{
	    page_table2[i]=page_table2[i] & 0xFFFFFFFE;
	}
    }
}


void
HabilitarPaginas(proceso_t * proc)
{
    int i;
    proceso_t * procAux;
    for(i=0;i<MAX_PAGES-1;i++)
    {
	if(mem[i]==proc->pid || mem[i]==proc->padre )
	{
	    page_table2[i]=page_table2[i] | 3;
	}
    }
}


void
DeshabilitarPaginas(proceso_t * proc)
{
    int i;
    for(i=0;i<MAX_PAGES-1;i++)
    {
	if(mem[i]==proc->pid && (proc->pid!=INIT && mem[i]==proc->padre))
	{
	    page_table2[i]=page_table2[i] & 0xFFFFFFFE;
	}
    }
}
