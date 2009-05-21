#include "../include/paging.h"
int mem[MAX_PAGES];
unsigned long *page_directory = (unsigned long *) 0x400000;
unsigned long *page_table = (unsigned long *) 0x401000;
unsigned long *page_table1;
unsigned long *page_table2;

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
    unsigned int i, j;

    /*El kernel como la tabla de directorio de paginas como las tablas de paginas
      se mapean en los primeros 8MB de memoria.*/
    for (i = 0; i < 2; i++) {
	/*Lleno la tabla de paginas con la informacion pertinente.*/
	
        for (j = 0; j < 1024; j++) {
	    /*La pagina tendra los siguientes atributos: Nivel supervisor,
	      read/write y presente. Se representa como 011 en binario.*/
            page_table[j] = address | 3;
            address = address + 4096;
        }
	/*Lleno la tabla de directorio de paginas con la informacion pertinente.*/
        page_directory[i] = (unsigned long)page_table;
        page_directory[i] = page_directory[i] | 3;
	page_table = page_table + 4096;

    }
    page_table = page_directory + 8192;

    /*Mapeo las paginas que estaran destinadas a ser usadas por las aplicaciones en
      nivel usuario*/
    for (i = 2; i < (int)3; i++) {
	/*Lleno la tabla de paginas con la informacion pertinente.*/
	
	for (j = 0; j < 1024; j++) {
	    /*Marco la pagina como no presente*/
            page_table[j] = address |3/*& 0xFFFFFFFE*/;
            address = address + 4096;
        }
	if(i==2)
	    page_table1=page_table;
	/*Lleno la tabla de directorio de paginas con la informacion pertinente.*/
        page_directory[i] = (unsigned long) page_table;
        page_directory[i] = page_directory[i] | 3 ;
	page_table = page_table + 4096;
    }
    
    /*Estas no son usadas por el SO ni las aplicaciones*/
    for (i = 3; i < (int)8; i++) {
	
        /*Lleno la tabla de paginas con la informacion pertinente.*/
        for (j = 0; j < 1024; j++) {

            page_table[j] = address & 0xFFFFFFFE;
            address = address + 4096;
        }
        page_directory[i] = (unsigned long) page_table;
        page_directory[i] = page_directory[i] & 0xFFFFFFFE;
	page_table = page_table + 4096;
    }

    /*En el registro CR3 guardo cual es el directorio de paginas.
      Seteo el bie correspondiente en CR0 para habilitar la paginacion.*/
    write_cr3 (page_directory);
    write_cr0 ((unsigned long *)(read_cr0 () | 0x80000000));
}

/*Habilito las paginas pertenecientes a un proceso.*/
void
habilitarPagina(proceso_t * proc)
{
    return;
    int i;
    for(i=0;i<MAX_PAGES;i++)
    {
	if(mem[i]==proc->pid)
	{
	    page_table1[i]=page_table1[i] | 3;
	}
    }
}

/*Deshabilito las paginas pertenecientes a un proceso.*/
void
deshabilitarPagina(proceso_t * proc)
{
    return;
    int i;
    for(i=0;i<MAX_PAGES;i++)
    {
	if(mem[i]==proc->pid)
	{
	    page_table1[i]=page_table1[i] & 0xFFFFFFFE;
	}
    }
}

