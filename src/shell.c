#include "../include/shell.h"
#include "../include/video.h"
#include "../include/defs.h"
#include "../include/pci.h"
#include "../include/kasm.h"
#include "../include/programas.h"
#include "../include/string.h"
#include "../include/kernel.h"


enum {  VOID=-1, 
        CLEAR, 
        KEYSLA, 
        KEYSUS, 
        LISTPCI, 
        HELP, 
        IMPRIMELETRA,
        IMPRIMELETRA_BK,
        IMPRIME4EVER,
        IMPRIME4EVER_BK,
        KILL,
        TOP,
        TOP_BK,
        CALCULA1,
        CALCULA1_BK,
        CALCULA2,
        CALCULA2_BK,
        CALCULA3,
        CALCULA3_BK,
        LLENASTACK,
        LLENASTACK_BK,
        DIVIDIRCERO,
        FORTUNE,
	PAGEFAULT,
	PAGINAS,
	MUCHOSPROC,
	NOTFOUND,
     };
#define NCOM 25

int
strcmp(char* s1, char* s2)
{
	while( *s1 != '\0' && *s2 != '\0') {
		if( *s1 < *s2 )
			return -1;
		if( *s1 > *s2 )
			return 1;
		s1++;
		s2++;
	}

	if( *s1 == '\0' && *s2 == '\0')
		return 0;
	if( *s1 == '\0')
		return -1;
	if( *s2 == '\0')
		return 1;
	return 0;
}

int 
ParsearArgumentos(char * line)
{
    int pid;
    pid = atoi(line + 5);
    return pid;

}

int
command(char *line )
{
  char * comlist[] = { "clear", "loadkeys la", "loadkeys us", "lspci", "?", "imprime", "imprime&", "imprime4ever", "imprime4ever&", "kill", "top", "top&", "calcula1",
    "calcula1&", "calcula2", "calcula2&", "calcula3", "calcula3&","llenastack", "llenastack&",
"dividirporcero","fortune","pagefault","paginas","muchosprocesos"};

        int pid;
	if( line[0] == '\0' )
		return VOID;

	int i;
	for( i=CLEAR; i<NCOM; i++ ) {
		if( strcmp(line,comlist[i]) == 0 )
			return i;
	}

        if(strncmp(line, "kill ", 5 ) == 0)
        {
            pid = ParsearArgumentos(line);
            if(pid >= 0 )
                Kill(pid);
            else
                printf("bash: kill only accepts a positive integer as argument.\n");

            return VOID;
        }

	return NOTFOUND;
}



int
shell(int argc, char ** argv)
{
	/* Buffer de almacenamiento de caracteres */
	char in_buffer[MAX_CHARS];
	set_screen();
	clear_screen();
	welcome();
	while(1) {
		prompt();
		getline(in_buffer,MAX_CHARS);
		// proceso el comando almacenado en in_buffer
		bash(in_buffer);
	}

	return 1 ;
}

void
bash(char *line  )
{
	int fnumber;
	int i;
	fnumber = command(line);
	switch(fnumber)
	{
	    case VOID:
			break;
	    case CLEAR:
			clear_screen();
			break;
	    case HELP:
			help();
			break;
	    case LISTPCI:
			lspci();
			break;
	    case KEYSLA:
			loadkeysla();
			break;
	    case KEYSUS:
			loadkeysus();
		        break;    
	    case IMPRIMELETRA: 
			CrearProceso ("ImprimeLetra", ImprimeLetras, 0,(char **)0, DEF_PRIO, FALSE, DEF_STACKSIZE);
			break;
	    case IMPRIMELETRA_BK:
			CrearProceso ("ImprimeLetra", ImprimeLetras, 0, (char **)0, DEF_PRIO, TRUE, DEF_STACKSIZE);
			break;
	    case IMPRIME4EVER: 
			CrearProceso ("ImprimeLetra4ever", ImprimeLetras4Ever, 0, (char **)0, DEF_PRIO, FALSE, DEF_STACKSIZE);
			break;
	    case IMPRIME4EVER_BK:
			CrearProceso ("ImprimeLetra4ever", ImprimeLetras4Ever, 0,(char **)0, DEF_PRIO, TRUE, DEF_STACKSIZE);
			break;
	    case TOP:
			CrearProceso("Top", Top, 0, (char**)0, 1, FALSE, DEF_STACKSIZE);
			break;
	    case TOP_BK:
			CrearProceso("Top", Top, 0, (char**)0, DEF_PRIO, TRUE, DEF_STACKSIZE);
			break;
	    case KILL:
			printf("Kill: Kill expects arg0 valid process_id\n");
			break;
	    case CALCULA1:
			CrearProceso("calculaLowPrio", Calcula, 0,(char**)0, 4, FALSE, DEF_STACKSIZE);
			break;
	    case CALCULA1_BK:
			CrearProceso("calculaLowPrio", Calcula, 0,(char**)0, 4, TRUE, DEF_STACKSIZE);
			break;
	    case CALCULA2:
			CrearProceso("calculaMedPrio", Calcula, 0,(char**)0, 2, FALSE, DEF_STACKSIZE);
			break;
	    case CALCULA2_BK:
			CrearProceso("calculaMedPrio", Calcula, 0,(char**)0, 2, TRUE, DEF_STACKSIZE);
			break;
	    case CALCULA3:
			CrearProceso("calculaHiPrio", Calcula, 0,(char**)0, 0, FALSE, DEF_STACKSIZE);
			break;
	    case CALCULA3_BK:
			CrearProceso("calculaHiPrio", Calcula, 0,(char**)0, 0, TRUE, DEF_STACKSIZE);
			break;
	    case LLENASTACK:
			CrearProceso("LlenaStack", LlenarStack, 0,(char**)0, 1, FALSE, DEF_STACKSIZE);
			break;
	    case LLENASTACK_BK:
			CrearProceso("LlenaStack", LlenarStack, 0,(char**)0, 1, TRUE, DEF_STACKSIZE);
			break;
	    case FORTUNE:
			CrearProceso("Fortune", Fortune, 0,(char**)0, 1, FALSE, DEF_STACKSIZE);
			break;
	    case DIVIDIRCERO:
			CrearProceso("DividirPorCero", DividirPorCero, 0,(char**)0, 1, FALSE, DEF_STACKSIZE);
			break;
	    case PAGEFAULT:
			CrearProceso("PageFault", PageFaultTest, 0,(char**)0, 1, FALSE, DEF_STACKSIZE);
			break;
	    case PAGINAS:
			CrearProceso("Paginas", Paginas, 0,(char**)0, 1, FALSE, DEF_STACKSIZE);
			break;
	    case MUCHOSPROC:
			for(i=0;i<60;i++)
			{
			    CrearProceso("Proceso vacio", Vacio, 0,(char**)0, 1, TRUE, DEF_STACKSIZE);
			}
			break;
	    default: 	printf("bash: ");
			printf(line);
			printf(": command not found -- Try ? for help\n");
	}

}
