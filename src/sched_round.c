#include "../include/defs.h"
#include "../include/kc.h"
#include "../include/paging.h"
#define CUANTUM_MAX 6

extern proceso_t procesos[];
extern int pidActual;
extern int maxmem;
int termina=0;
int indice = 0;
int ultimos100[100] = { 0 };
int nivelActual = 0;
int actual = 0;
int vuelta = 0;

void
ActualizarPorcentajesCPU(void)
{
    int i;
    ReiniciarPorcentajes();
    for(i = 0;i<100;i++)
    {
        procesos[ultimos100[i]].cpu++;
    }
}

void
GuardarESP (int ESP)
{
    proceso_t *temporal;
    temporal = TraerProcesoPorPid(pidActual);
    temporal->ESP = ESP;
    if((ESP - (temporal->stackstart - temporal->stacksize))<500 && pidActual!=INIT && !termina)
    {
        temporal->stackstart  =(int) KRealloc(temporal, temporal->stacksize/PAGE_SIZE + 1);
	if(temporal->stackstart==0)
	{
	    Kill(pidActual);
	    switch_manual();
	}
    }
    termina=0;
    return;
}

int
CargarESP (proceso_t * proc)
{
    return proc->ESP;
}

int
SiguienteProceso (int esp)
{
    proceso_t *temporal;
    temporal=TraerProcesoPorPid(pidActual);
    if(temporal->pid<0)
    {
	temporal = SiguienteTarea();
	
	pidActual = temporal->pid;

	HabilitarPaginas(temporal);
    }
    else
    {
	GuardarESP (esp);
	temporal = SiguienteTarea();

	DeshabilitarPaginas(TraerProcesoPorPid(pidActual));

	pidActual = temporal->pid;

	HabilitarPaginas(temporal);
    }

    return temporal->ESP;
}

proceso_t *
SiguienteTarea (void) {
    int i;
    int tengoProceso = 0;
    int cant = 0;
    /* Se decrementa el tiempo a dormir de los procesos*/
    for(i = 1; i < MAXPROCESOS; i++)
    {
	if(procesos[i].sleep > 0)
	{
	    if(--procesos[i].sleep == 0)
	    {
		procesos[i].estado = LISTO;
	    }
	}	
    }
    
    /*Si no hay ningun proceso listo para ser ejecutado devuelve al proceso INIT.*/
    if (NoHayProcesos ()) {

        ultimos100[indice] = INIT;
        indice = (indice + 1) % 100;
        return &procesos[INIT];
    }

    i = ((actual + 1) % MAXPROCESOS < 1) ? 1 : (actual + 1) % MAXPROCESOS;
    
    /*Devuelvo el siguiente proceso que esta listo para ejecutarse.*/
    while (!tengoProceso) {
      if (!procesos[i].free_slot && procesos[i].estado ==LISTO && procesos[i].pid != INIT) {
                actual = i;
                ultimos100[indice] = i;
                indice = (indice + 1) % 100;
                return &procesos[i];
        }
        cant++;
        i = (i + 1) % MAXPROCESOS < 1 ? 1 : (i + 1) % MAXPROCESOS;
    }
}

void
IniciarMultiTarea (void) {
    int i;
    /*Marco todos los slots como libres*/
    for (i = 0; i < MAXPROCESOS; i++)
        procesos[i].free_slot = TRUE;
}
