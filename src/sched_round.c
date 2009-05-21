#include "../include/defs.h"
#include "../include/kc.h"

#define CUANTUM_MAX 6

extern proceso_t procesos[];
extern int pidActual;
extern int maxmem;

int indice = 0;
int ultimos100[100] = { 0 };
int nivelActual = 0;
int actual = 0;
int vuelta = 0;

void
ActualizarPorcentajesCPU(void)
{
    int i,j;
    proceso_t * proceso;
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
    GuardarESP (esp);
    temporal = SiguienteTarea();
    
    deshabilitarPagina(TraerProcesoPorPid(pidActual));

    pidActual = temporal->pid;
    
    habilitarPagina(temporal);

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
