#include "../include/defs.h"
#include "../include/kc.h"
#include "../include/random.h"


extern proceso_t procesos[];
extern int pidActual;
extern  int maxmem;


int posiblesGanadores[100] = {0};


int indice = 0;
int ultimos100[100] = { 0 };
int nivelActual = 0;
int actual = 0;
int vuelta = 0;
static int CalcularProporciones(void);

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
GuardarESP (int ESP) {

    proceso_t *temporal;
    temporal = TraerProcesoPorPid(pidActual);
    temporal->ESP = ESP;
    return;
}


/* siguiente proceso a ejecutar */
int
SiguienteProceso (int esp) {
    proceso_t *temporal;
    GuardarESP (esp);
    temporal = SiguienteTarea();
   // bajarPaginasAnterior(pidActual);
   
    pidActual = temporal->pid;
//    levantaPaginasNuevo(pidActual);
    return temporal->ESP;
}

/* la papota */

proceso_t *
SiguienteTarea (void) {
    int i;
    int tengoProceso = 0;
    int cant = 0;
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

    ganador = random(100);
    //printf("Ganador:  %d pg %d\n", ganador, posiblesGanadores[ganador]);
   // int j;
  //  for(j = 0; j < 100;j=j+2)
    //    printf("%d \t %d\n", posiblesGanadores[j], posiblesGanadores[j+1]);
  //  _debug();
    //TODO validar q procesoGanador no sea null y q TraerIndiceProceso no de -1.
    procesoGanador = TraerProcesoPorPid(posiblesGanadores[ganador]);

    ultimos100[indice] = TraerIndiceProceso(procesoGanador->pid);
    indice = (indice + 1)%100;

    return procesoGanador;
}

int
CargarESP (proceso_t * proc) {
    return proc->ESP;
}

void
IniciarMultiTarea (void) {
    int i;

 //   void *stack = Malloc (512);
   /*Hay que busfcar una direccion perteneciente a la zona de kernel*/
    void *stack = (void *)0x300000;
    
    /* todos los slots estan libres */
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
            tickets += (AJUSTE_PRIO - procesos[i].prioridad);
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
            ticketsPorProceso = (int)(((AJUSTE_PRIO - procesos[i].prioridad)/ tickets) * 100);

//printf("\t\t tickets %d      prio %d\n", tickets,(int)((100*3)/(float)4));
//printf("\t\t tickPorProceso %d     pid %d\n", ticketsPorProceso, procesos[i].pid);

            for(j = 0; j < ticketsPorProceso; j++)
            {
                posiblesGanadores[ixProporc] = procesos[i].pid;
                ixProporc++;
            }
        }
    }
//_debug();
    return tickets;
}

