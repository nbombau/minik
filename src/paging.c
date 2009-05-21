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
    unsigned long address = 0;  // holds the physical address of where a page is
    unsigned int i, j;

    // map the kernel pages and table directories and table pages zone (first 8MB)
    for (i = 0; i < 2; i++) {
        //page_table = page_table + 4096;

        for (j = 0; j < 1024; j++) {
            // fill  the page directory
            page_table[j] = address | 3;
         // attribute set to: supervisor level, read/write, present(011 in binary)
            address = address + 4096;
        }

        page_directory[i] = (unsigned long)page_table;
        page_directory[i] = page_directory[i] | 3;
	page_table = page_table + 4096;

    }
    page_table = page_directory + 8192;

    for (i = 2; i < (int)4; i++) {
       for (j = 0; j < 1024; j++) {

            page_table[j] = address & 0xFFFFFFFE;
            address = address + 4096;
        }
	if(i==2)
	    page_table1=page_table;
	if(i==3)
	    page_table2=page_table;
        page_directory[i] = (unsigned long) page_table;
        page_directory[i] = page_directory[i] | 3 ;
	page_table =  page_table + 4096;

    }
    
    /*Estas no son usadas por el SO ni las aplicaciones*/
    for (i = 4; i < (int)8; i++) {
        

        for (j = 0; j < 1024; j++) {

            page_table[j] = address & 0xFFFFFFFE;
            address = address + 4096;
        }
        page_directory[i] = (unsigned long) page_table;
        page_directory[i] = page_directory[i] & 0xFFFFFFFE ;
	page_table =  page_table + 4096;

    }

   // write_cr3, read_cr3, write_cr0, and read_cr0 all come from the 
    // assembly functions
    write_cr3 (page_directory); // put that page directory address into CR3
    write_cr0 ((unsigned long *)(read_cr0 () | 0x80000000));   // set the paging bit in CR0 to 1
}

void
habilitarPagina(proceso_t * proc)
{
    int i;
    for(i=0;i<MAX_PAGES;i++)
    {
	if(mem[i]==proc->pid)
	{
	    
	    if(i<1024)
	    {
		//page_table =(unsigned long*) page_directory[2];
		//page_directory[2] = page_directory[2] | 7;
		//(page_table+4*PAGE_SIZE)[i]=(page_table+4*PAGE_SIZE)[i] | 7;
		page_table1[i]=page_table1[i] | 3;
	    }
	    else
	    {
		page_table2[i]=page_table2[i] | 3;
		//page_directory[3] = page_directory[3] | 7;
		//(page_table+3*PAGE_SIZE)[i]=(page_table+3*PAGE_SIZE)[i] | 7;
	    }
	}
    }
}

void
deshabilitarPagina(proceso_t * proc)
{
    int i;
    /*if(proc->pid==0)
	return;*/
    for(i=0;i<MAX_PAGES;i++)
    {
	if(mem[i]==proc->pid)
	{
	    
	    if(i<1024)
	    {
		//page_table =(unsigned long*) page_directory[2];
		//page_directory[2] = page_directory[2] | 7;
		//(page_table+4*PAGE_SIZE)[i]=(page_table+4*PAGE_SIZE)[i] | 7;
		page_table1[i]=page_table1[i] & 0xFFFFFFFE;
	    }
	    else
	    {
		page_table2[i]=page_table2[i] & 0xFFFFFFFE;
		//page_directory[3] = page_directory[3] | 7;
		//(page_table+3*PAGE_SIZE)[i]=(page_table+3*PAGE_SIZE)[i] | 7;
	    }
	}
    }
}
