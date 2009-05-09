#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/ints.h"
#include "../include/kc.h"
#include "../include/pci.h"
#include "../include/video.h"
#include "../include/shell.h"


DESCR_INT idt[0x81];			/* IDT de 10 entradas*/
IDTR idtr;				/* IDTR */

proceso_t procesos[MAXPROCESOS];
proceso_t init;



int pidActual = 0;
int proxPid = 1;
int maxmem;

/**********************************************
kmain()
Punto de entrada de cóo C.
*************************************************/

int Init(int argc, char **argv)
{
  while(1) {
    asm volatile ("hlt");
  }
}


kmain()
{

        int i,num;
        int flag = 0;
        int vendor = 0xffff;
        _Cli();
        IniciarMultiTarea();
/* CARGA DE IDT CON LA RUTINA DE ATENCION DE IRQ0 y IRQ1   */
        // Timer Tick
        setup_IDT_entry (&idt[0x08], 0x08, (dword)&_int_08_hand, ACS_INT, 0);
        setup_IDT_entry (&idt[0x80], 0x08, (dword)&_int_80_hand, ACS_INT, 0);

        /* Por default el teclado esta en ingles */
        setup_IDT_entry (&idt[0x09], 0x08, (dword)&_int_09_hand_US,ACS_INT, 0);
        
        setup_IDT_entry (&idt[0x00], 0x08, (dword)&div0_hand, ACS_INT, 0);
    // out of bounds
        setup_IDT_entry (&idt[0x05], 0x08, (dword)&bounds_hand, ACS_INT, 0);
    // opcode invalido
        setup_IDT_entry (&idt[0x06], 0x08, (dword)&opCode_hand, ACS_INT, 0);    
    // segmento no presente
        setup_IDT_entry (&idt[0x0B], 0x08, (dword)&snoPresent_hand, ACS_INT, 0);
    // ay se me rompe el stack!
        setup_IDT_entry (&idt[0x0C], 0x08, (dword)&ssf_hand, ACS_INT, 0);   
    // error de prot. general
        setup_IDT_entry (&idt[0x0D], 0x08, (dword)&generalPfault_hand, ACS_INT, 0); 
    // saca la mano antonio de mi pagina
        setup_IDT_entry (&idt[0x0E], 0x08, (dword)&pageFault_hand, ACS_INT, 0); 

        /* Carga de IDTR    */

        idtr.base = 0;
        idtr.base +=(dword) &idt;
        idtr.limit = sizeof(idt)-1;

        _lidt (&idtr);

        

        /* Habilito interrupcion del teclado */

		byte aux = 0xFD;
        write(PICM1,&aux,1 );
        aux = 0xFF;
        write(PICM2,&aux,1 );

        CrearProceso ("shell", shell, 0, (char **) 0, 2, 0, 0x256);

      //  CrearProceso ("INIT", Init, 0, (char **)0, 2, 0, 0x4096);
      //  desbloqueaProceso(INIT);
        probarMemoria(0, (char ** )0);
        _Sti();


        while (1) {
        }
        


}

void
loadkeysla(void)
{
	setup_IDT_entry (&idt[0x09], 0x08, (dword)&_int_09_hand_LAT,ACS_INT, 0);
}

void
loadkeysus(void)
{
	setup_IDT_entry (&idt[0x09], 0x08, (dword)&_int_09_hand_US,ACS_INT, 0);
}

void
reboot(void)
{
	int i;
	/* Blanqueo el idt */
	for (i=0; i<10; i++)
	        setup_IDT_entry (&idt[i], 0x08, 0, ACS_INT, 0);

	_Fault();
}
