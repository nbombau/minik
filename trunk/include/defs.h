/***************************************************
  Defs.h

****************************************************/

#ifndef _defs_
#define _defs_


typedef enum { SCREEN, KEYBOARD, PCI, CURSOR1, CURSOR2,  SCREENNL, PIC, PICM1, PICM2, KEYBOARD_IN } F_DESCRIPTOR;

#define byte unsigned char
#define word short int
#define dword int

/* Flags para derechos de acceso de los segmentos */
#define ACS_PRESENT     0x80            /* segmento presente en memoria */
#define ACS_CSEG        0x18            /* segmento de codigo */
#define ACS_DSEG        0x10            /* segmento de datos */
#define ACS_READ        0x02            /* segmento de lectura */
#define ACS_WRITE       0x02            /* segmento de escritura */
#define ACS_IDT         ACS_DSEG
#define ACS_INT_386 	0x0E		/* Interrupt GATE 32 bits */
#define ACS_INT         ( ACS_PRESENT | ACS_INT_386 )


#define ACS_CODE        (ACS_PRESENT | ACS_CSEG | ACS_READ)
#define ACS_DATA        (ACS_PRESENT | ACS_DSEG | ACS_WRITE)
#define ACS_STACK       (ACS_PRESENT | ACS_DSEG | ACS_WRITE)

#pragma pack (1) 		/* Alinear las siguiente estructuras a 1 byte */

/* Ctes para video*/
#define CANT_COLS 80
#define CANT_ROWS 25
#define VID_CHAR 2

// defines sacados a partir de http://docs.hp.com/en/65/books/uidatt.htm
#define WHITE_TXT 0x07	// Atributo de video. Letras blancas, fondo negro
#define RED_TXT	0x74	// Atributo de video. Letras rojas, fondo gris

/*Ctes para teclado*/
#define MAX_CHARS 200
#define EMPTY -1
#define READ 1

/*scancodes particulares*/
#define ESC 0x01
#define LSHIFT 0x2A
#define RSHIFT 0x36
#define CAPSLOCK 0x3A
#define NUMLOCK 0x45
#define SCROLLLOCK 0x46
#define BACKSPACE 0x0E

#define IS_MAKE_CODE(c) (c)<128
#define IS_BREAK_CODE(c) (c)>=128

/*Mascara para saber si el codigo es un break code
 *breakcode=scancode|0x0080*/
#define MASK 0x0080


#define MAX_CANT 100

#define MAX_NOMBRE_PROCESO  32

#define MAXPROCESOS 64

/* Pid Proceso Init
*/
#define INIT 0

#define SHELL 1

/*Prioridad Default
*/
#define DEF_PRIO    2

/* Constante para scheduler por prioridades
*/
#define AJUSTE_PRIO  5

/* Tamaño Default del Stack
*/

#define DEF_STACKSIZE 0x1000

/* Constante de tiempo que representa el tiempo del timer tick
*  Se utiliza para que la funcion sleep utilice los segundos
*  como medida de tiempo.
*/

#define TIMER_TICK 18

/*
* Estados de Procesos
*/

#define LISTO 2
#define EJECUTANDO 3
#define ESPERANDO_HIJO 4
#define BLOQUEADO 5

#define TRUE   1
#define FALSE  0
#define ERROR -1

/* Maxima cantidad de paginas. Se mapean 8 MB
*/

#define MAX_PAGES 1024

#define FIRST_USER_PAGE 8388608

#define PAGE_SIZE 4096

/* Procesos 
*/
typedef struct {
  int pid;
  char nombre[MAX_NOMBRE_PROCESO];
  int prioridad;
  int background;
  int estado;
  int padre;
  int ESP;
  int free_slot;
  int stackstart;
  int stacksize;
  int nextfree;
  long sleep;
  int cpu;
} proceso_t;


/* Ctes para PCI */
typedef unsigned int UINT32;

/* Descriptor de segmento */
typedef struct {
  word limit,
       base_l;
  byte base_m,
       access,
       attribs,
       base_h;
} DESCR_SEG;


/* Descriptor de interrupcion */
typedef struct {
  word      offset_l,
            selector;
  byte      cero,
            access;
  word	    offset_h;
} DESCR_INT;

/* IDTR  */
typedef struct {
  word  limit;
  dword base;
} IDTR;

typedef struct 
{
	word limit;
	dword base;
}GDTR;

typedef int FileDesc;


#endif

