#include "../include/video.h"
#include "../include/defs.h"
#include "../include/ints.h"
#include "../include/kasm.h"
#include "../include/buffer.h"

/* Variable global de la posicion en pantalla */
int screen_pos = 0;

int
writeWrapper(const void * buff, int size)
{
	//FileDesc fd;
	char auxc = ' ';
	int i;
	int c;

	if (screen_pos >= (CANT_COLS * CANT_ROWS)*2)
	{
		for (screen_pos = 0; screen_pos < CANT_COLS *(CANT_ROWS -1)*2;)
		{
			c = getNLChar(SCREENNL);
			write (SCREEN, &c, 1);
		}
		for(i = 0; i < CANT_COLS; i++)
			write (SCREEN, &auxc, 1);
		screen_pos = CANT_COLS *(CANT_ROWS -1)*2;
	}
	for (i = 0; i < size; i++)
	{
		if (((char*)buff)[i] == '\n')
		{
			screen_pos += (CANT_COLS*2) - (screen_pos % (CANT_COLS *2));
		}
		else if (((char*)buff)[i] == '\t')
		{
			screen_pos += 4;
		}
		else if (((char*)buff)[i] == '\b')
		{
			screen_pos -= 2;
			write (SCREEN,&auxc,1);
			screen_pos -= 2;
		}
		else
		{
			write (SCREEN,buff+i,1);
		}
	}
	move_cursor(screen_pos/2);

    return size;
}

void
clear_screen()
{
	char c = ' ';
	unsigned int i=0;

	screen_pos = 0;

	while(i < (80*25)){
		i++;
		putchar(c);
	};
	screen_pos = 0;
}

void
putchar(char c)
{
	writeWrapper(&c,1);
}

int
getNLChar()
{
	char c;
	read(SCREENNL, &c, 1);
	return c;
}

void
getline(char *buffer, int max)
{
    char c[1];
    int timer=0;
    int last = EMPTY;

    do {
		if( !BufferIsEmpty() ) {
		/* chequea y lee en caso de haber una entrada pendiente */
			read(KEYBOARD,&(c[0]),1);

			/* Chequeo que al recibir un backspace no borre mas alla
			 * del prompt y borre del buffer de salida al caracter
			 * correspondiente
			 */
			if( c[0] == '\b' && last != EMPTY ) {
				buffer[last] = ' ';
				last--;
				putchar(c[0]);
			}
			if( c[0] != '\b' && c[0] != '\n' ) {
				if( last < max - 1  ) {
					buffer[++last] = c[0];
					putchar(c[0]);
				}
			}
			/* el cambio de linea no lo guardo pero si lo imprimo */
			if( c[0] == '\n' ) {
				putchar(c[0]);
				++last;
			}
        }
    } while( c[0] != '\n'  );

	/* Completo el string  */
	buffer[last] = '\0';
}

void
printLine(char *line)
{
	while( *line != '\n' ) {
		putchar(*line);
		line++;
	}
}

void
welcome()
{
    printf(" __  __ _       _ _  __    ____________________________________________\n");
    printf("|  \\/  (_)_ __ (_) |/ /    Minikernel v2.0                             \n");
    printf("| |\\/| | | '_ \\| | ' /     Version: Mayo-09                        \n");
    printf("| |  | | | | | | | . \\     ITBA - Sistemas Operativos         \n");
    printf("|_|  |_|_|_| |_|_|_|\\_\\    ____________________________________________\n");
    printf("                      	                                                       \n");
    printf("type ? for help                                                                \n");
	printf("\n");
}

void
help()
{

	printf("_______________________________________________________________________________\n");
	printf("\n");
	printf("                              Comandos disponibles                             \n");
	printf("\n");
	printf(" clear            : limpia la pantalla                                         \n");
	printf(" lspci            : lista los dispositivos pci disponibles                     \n");
	printf(" loadkeys [us,la] : cambia el idioma del teclado                               \n");
	printf(" reboot           : reinicia el sistema                                        \n");
	printf("_______________________________________________________________________________\n");
	printf("\n");
}

void
print(char *string)
{
	_Cli();
	while( *string != '\0' ) {
		putchar(*string);
		string++;
	}
	_Sti();
}

void
prompt()
{
	printf(">: ");
}

void
set_screen(void)
{
	screen_pos = 0;
	move_cursor(0);
}

void
move_cursor(unsigned short int pos)
{
	byte aux;

	aux = 0x0F;
	write(CURSOR1, &aux, 1);

	aux = pos&0xFF;
	write(CURSOR2, &aux, 1);

	aux = 0x0E;
	write(CURSOR1, &aux, 1);

	aux = (pos>>8)&0xFF;
	write(CURSOR2, &aux, 1);
}







