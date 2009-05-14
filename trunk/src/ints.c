#include "../include/defs.h"
#include "../include/kasm.h"
#include "../include/ints.h"
#include "../include/buffer.h"

#define CANT_KEYS 104
#define ALTGR 0x38
#define IS_NUMPAD_KEY(c) ((c)>=0x47 && (c)<=0x53)
#define IS_VALID_ASCII(c) (c)!=0

extern screen_pos;

/* mapeo de los estados del teclado */
int numlock=0,caps=0,scrolllock=0,shift=0,altgr=0;

/* Contador del cursor */
int counter=0;
char *video = (char *) 0xb8000;
void
int_08()
{
}

int
int_09_US(unsigned char code)
{
	/*Veo que teclas estan oprimidas y modifico las variables
		 *globales, en caso de ser las teclas scrolllock,capslock
		 *o numlock */
	    char ascii;
	    if( code==LSHIFT || code==RSHIFT)
			shift=1;
		else if( code==(LSHIFT | MASK) || code==(RSHIFT | MASK) )
			shift=0;
		else if( code==CAPSLOCK )
			caps=!caps;
		else if( code==NUMLOCK )
			numlock=!numlock;
		else if( code==SCROLLLOCK )
			scrolllock=!scrolllock;
	        else if( IS_NUMPAD_KEY(code) && !numlock)
	            return 0;
		else
		{
			if( IS_MAKE_CODE(code) && code<=CANT_KEYS)
			{
			    ascii=ToAsciiUS(code);
			    if(IS_VALID_ASCII(ascii))
			        AddToBuffer(ascii);
			}
		}
		return 0;
}


int
int_09_LAT(unsigned char code)
{
	/*Veo que teclas estan oprimidas y modifico las variables
		 *globales, en caso de ser las teclas scrolllock,capslock
		 *o numlock */
	   char ascii;
	    if( code==LSHIFT || code==RSHIFT)
			shift=1;
		else if( code==(LSHIFT | MASK) || code==(RSHIFT | MASK) )
			shift=0;
		else if( code==CAPSLOCK )
			caps=!caps;
		else if( code==NUMLOCK )
			numlock=!numlock;
		else if( code==SCROLLLOCK )
			scrolllock=!scrolllock;
	        else if( IS_NUMPAD_KEY(code) && !numlock)
	            return 0;
		else
		{
			if( IS_MAKE_CODE(code) && code<=CANT_KEYS)
			{
			    ascii=ToAsciiLAT(code);
			    if(IS_VALID_ASCII(ascii))
			        AddToBuffer(ascii);
			}
		}
		return 0;
}

static void
s_write(const void* buff, int size)
{
    int i;
    //char *video = (char *) 0xb8000;

    _Cli();
    for(i = 0; i < size; i++)
    {
        video[screen_pos++]= *((char*)buff+i);
        video[screen_pos++]= WHITE_TXT;
    }
    _Sti();
}

static void
l_write(int p, const void *buff, int size)
{
	int i;
    _Cli();

    for(i = 0; i < size; i++) {
    	myoutl( p , ( (int *) buff )[i] );
    }
    _Sti();
}

static void
b_write(int p, const void *buff, int size)
{
	int i;
    _Cli();

    for(i = 0; i < size; i++) {
    	myout( p , ( (byte *) buff )[i] );
    }
    _Sti();
}



void
int_80w(FileDesc fd, const void * buff, int size)
{
	switch (fd)
	{
		case SCREEN:  s_write(buff,size); break;
		case PCI   :  l_write(0x0CF8, buff,size); break;
		case CURSOR1:  b_write(0x3D4, buff, size); break;
		case CURSOR2:  b_write(0x3D5, buff, size); break;
		case PIC: 	   b_write(0x20, buff, size); break;
		case PICM1:    b_write(0x21, buff, size); break;
		case PICM2:    b_write(0x0A1, buff, size); break;
	}
}

static void
k_read(char *buff, int size)
{
	int i=0;
	_Cli();
	while( !BufferIsEmpty() && i< size ) {

		buff[i] = GetFromBuffer() ;
		i++;
	}

	_Sti();

}

static void
s_read(char *buff, int size) {
	int i=0;

	char * video;
	video = (char *) 0xb8000;
	_Cli();

	while( i< size ) {

		buff[i] = *(video + screen_pos + CANT_COLS *2);
		i++;

	}

	_Sti();
}

static void
l_in(int p, int *buff, int size)
{
	int i;
    for(i = 0; i < size; i++) {
    	myinl( p, &(buff[i]) );
    }
}

static void
b_in(int p, byte *buff, int size)
{
	int i;
    for(i = 0; i < size; i++) {
    	myin( p, &(buff[i]) );
    }
}

void
int_80r(FileDesc fd, void * buff, int size)
{
	int i = 0;

	switch(fd)
	{
	case KEYBOARD: 	k_read((char *)buff,size); break;
	case SCREENNL: 	s_read((char *)buff,size); break;
	case PCI	 :  	l_in(0xCFC, buff,size);   break;
	case KEYBOARD_IN:	b_in(0x60,(byte *)buff, size); break;
	}
}

char
read_key(void)
{
	byte aux[1];
	read(KEYBOARD_IN,aux,1);

	return aux[0];
}

void
send_eoi(void)
{
	byte aux=0x20;
	write(PIC, &(aux), 1 );
}



