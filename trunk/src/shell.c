#include "../include/shell.h"
#include "../include/video.h"
#include "../include/defs.h"
#include "../include/pci.h"
#include "../include/kasm.h"
#include "../include/programas.h"


enum {  VOID=-1, 
        CLEAR, 
        KEYSLA, 
        KEYSUS, 
        LISTPCI, 
        HELP, 
        REBOOT, 
        IMPRIMELETRA,
        IMPRIMELETRA_BK,
        IMPRIME4EVER,
        IMPRIME4EVER_BK,
        KILL,
        NOTFOUND, 
     };
        
#define NCOM 12

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
}

int
command(char *line )
{
  char * comlist[] = { "clear", "loadkeys la", "loadkeys us", "lspci", "?", "reboot", "imprime", "imprime&", "imprime4ever", "imprime4ever&", "kill" };


	if( line[0] == '\0' )
		return VOID;

	int i;
	for( i=CLEAR; i<NCOM; i++ ) {
		if( strcmp(line,comlist[i]) == 0 )
			return i;
	}

	return NOTFOUND;
}

void
bash(char *line  )
{
	int fnumber;

	fnumber = command(line);
	switch(fnumber)
	{
	case VOID: break;
	case CLEAR: clear_screen();
				break;
	case HELP:  help();
				break;
	case LISTPCI:	lspci();
					break;
	case KEYSLA: loadkeysla();
				 break;
	case KEYSUS: loadkeysus();
		         break;
	case REBOOT: reboot();
    
    case IMPRIMELETRA: 
                CrearProceso ("ImprimeLetra", ImprimeLetras, 0, 
                    (char **)0, DEF_PRIO, FALSE, DEF_STACKSIZE);   
                break;
      case IMPRIMELETRA_BK:
        CrearProceso ("ImprimeLetraBK", ImprimeLetras, 0, 
                      (char **)0, DEF_PRIO, TRUE, DEF_STACKSIZE);   
        break;
      case IMPRIME4EVER: 
        CrearProceso ("ImprimeLetra4ever", ImprimeLetras4Ever, 0, 
                      (char **)0, DEF_PRIO, FALSE, DEF_STACKSIZE);   
        break;
      case IMPRIME4EVER_BK:
        CrearProceso ("ImprimeLetraBK", ImprimeLetras4Ever, 0, 
                      (char **)0, DEF_PRIO, TRUE, DEF_STACKSIZE);   
        break;
    case KILL:
                Kill(2);    
                break;
    
    

	default: 	printf("bash: ");
				printf(line);
				printf(": command not found -- Try ? for help\n");
	}

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

