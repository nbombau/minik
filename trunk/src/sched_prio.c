#include "../include/defs.h"
#include "../include/kc.h"
#include "../include/random.h"
#include "../include/paging.h"

extern proceso_t procesos[];
extern int pidActual;
extern  int maxmem;

int tiempoEsperaConsola = 0;

int posiblesGanadores[1600] = {0};


int indice = 0;
int ultimos100[100] = { 0 };
int nivelActual = 0;
int actual = 0;
int vuelta = 0;
static int CalcularProporciones(void);

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
    if((ESP - (temporal->stackstart - temporal->stacksize))<500 && pidActual!=INIT)
    {
        temporal->stackstart  =(int) KRealloc(temporal, temporal->stacksize/PAGE_SIZE + 1);
    }

    return;
}

int
SiguienteProceso (int esp)
{
    proceso_t *temporal;

    GuardarESP (esp);
    temporal = SiguienteTarea();

    DeshabilitarPaginas(TraerProcesoPorPid(pidActual));

    pidActual = temporal->pid;

    HabilitarPaginas(temporal);

    return temporal->ESP;
}

proceso_t *
SiguienteTarea (void)
{
    int i;
    int tickets;
    int ganador;
    proceso_t *procesoGanador;
    /* Se decrementa el tiempo a dormir de los procesos
    */
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

    if (NoHayProcesos ()) {

        ultimos100[indice] = INIT;
        indice = (indice + 1) % 100;
        return &procesos[INIT];
    }

    tickets = CalcularProporciones();

    ganador = random(1600);

    //TODO validar q procesoGanador no sea null y q TraerIndiceProceso no de -1.
    procesoGanador = TraerProcesoPorPid(posiblesGanadores[ganador]);

    ultimos100[indice] = TraerIndiceProceso(procesoGanador->pid);
    indice = (indice + 1)%100;

    return procesoGanador;
}

void
IniciarMultiTarea (void)
{
    int i;

    /*Marco todos los slots como libres*/
    for (i = 0; i < MAXPROCESOS; i++)
        procesos[i].free_slot = TRUE;
}

static int CalcularProporciones(void)
{
    int i,j,tickets = 0;
    int ixProporc = 0;
    int ticketsPorProceso;
    for(i = 1; i<MAXPROCESOS;i++)
    {
        if(!procesos[i].free_slot && procesos[i].estado == LISTO)
        {
            tickets += ((AJUSTE_PRIO - procesos[i].prioridad)*(AJUSTE_PRIO - procesos[i].prioridad));
        }
    }

    
    for(i = 1; i<MAXPROCESOS;i++)
    {
        if(!procesos[i].free_slot && procesos[i].estado == LISTO)
        {
            /* En honor al sabio inventor de la regla de 3, 
            *  cada proceso recibe una cantidad de tickets proporcional
            *  a su prioridad.
            */
            ticketsPorProceso = (int)((((AJUSTE_PRIO - procesos[i].prioridad)*(AJUSTE_PRIO - procesos[i].prioridad))*1600)/ tickets);

            for(j = 0; j < ticketsPorProceso; j++)
            {
                posiblesGanadores[ixProporc] = procesos[i].pid;
                ixProporc++;
            }
        }
    }
    return tickets;
}

