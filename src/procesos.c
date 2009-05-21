#include "../include/defs.h"
#include "../include/kc.h"
#include "../include/string.h"


#define STACK_USUARIOS  0x1000
#define DEF_PRIORITY    3

extern proceso_t procesos[];

extern int pidActual;
extern int proxPid;
int veces = 0;
//extern unsigned long *page_directory;
//extern unsigned long *page_table;

int probarMemoria(int argc, char **argv)
{ 
   char * a = (char *) 0xF00000;
   int i = 0;
   while( i < 99999)
        a[i++] = 'a';
return 0;

}


int
ProcesoExiste(int pid)
{
    int i;

    if (pid == 0)
        return 1;

    for (i = 0; i < MAXPROCESOS; i++) {
        if (procesos[i].pid == pid && !procesos[i].free_slot)
            return 1;
    }

    return 0;

}

int
CantidadDeProcesos(void)
{
    int i, cant = 0;
    for(i = 0; i < MAXPROCESOS; i++)
    {
        if(!procesos[i].free_slot)
            cant++;
    }
    return cant;
}

void
ReiniciarPorcentajes(void)
{
    int i;
    for(i = 0; i < MAXPROCESOS; i++)
    {
        procesos[i].cpu = 0;
    }
}


proceso_t *
TraerProcesoPorPid (int pid) {
    int i;

    for (i = 0; i < MAXPROCESOS; i++) {
        if (procesos[i].pid == pid && !procesos[i].free_slot)
            return &procesos[i];
    }

    return 0;
}

proceso_t*
TraerProcesoPorIndice(int i)
{
    return &procesos[i];
}

proceso_t *
TraerProcesoPorNombre (char *proceso) {
    int i;
    int len1, len2;

    len1 = strlen (proceso);

    for (i = 0; i < MAXPROCESOS; i++) {
        len2 = strlen(procesos[i].nombre);

        if (strncmp (proceso, procesos[i].nombre, len1) == 0 && len1 == len2
            && !procesos[i].free_slot) {
            return &procesos[i];
        }
    }

    return 0;
}

int
TraerIndiceProceso(int pid)
{
    int i;
    for(i = 1; i < MAXPROCESOS; i++)
    {
        if(procesos[i].pid == pid)
            return i;
    }
    return -1;
}

int
NoHayProcesos (void) {
    int i;

    for (i = 0; i < MAXPROCESOS; i++) {
        if (procesos[i].pid == INIT)
            continue;
        if (!procesos[i].free_slot && procesos[i].estado == LISTO)
            return 0;
    }
    return 1;
}

int
EstoyEnBackground(void)
{
    proceso_t * p = TraerProcesoPorPid(pidActual);
    return p->background;
}

int
NuevoPid (void) {
    return proxPid++;
}

void
exec (char *nombre, int (*proceso) (int argc, char **argv), int enBackground) {
    CrearProceso (nombre, proceso, 0, (char **) 0,
                  DEF_PRIORITY, enBackground, STACK_USUARIOS);
}

void
Limpia (void) {
    /* deberia matar al proceso y toda la bola */
    proceso_t *temporal;
    proceso_t *padre;
    _Cli();
    temporal = TraerProcesoPorPid (pidActual);

    if (temporal->padre != INIT) {
	padre = TraerProcesoPorPid (temporal->padre);
	padre->estado = LISTO;
    }
    temporal->free_slot = TRUE;
    _Sti();
    while (1) {
	asm volatile ("hlt");
    }
}

    
void
CrearProceso (char *nombre, int (*proceso) (int argc, char **argv),
              int argc, char **argv, int prioridad, int enBackground,
              int stacksize) {
    int i;

    void *stack;

    for (i = 0; i < MAXPROCESOS; i++) {
        if (procesos[i].free_slot)
            break;
    }

    veces++;
    strncpy (procesos[i].nombre, nombre, strlen (nombre)+1);
    procesos[i].pid = NuevoPid ();
    procesos[i].stacksize = PAGE_SIZE;
    //stack = (void *)(0xF00000 + (veces * 1024)) ;
    stack = (void *)KMalloc (&procesos[i]);

    procesos[i].background = enBackground;
    procesos[i].prioridad = prioridad;
    
    procesos[i].estado = LISTO;

    procesos[i].stackstart = (int) (stack +PAGE_SIZE - 1);
    procesos[i].sleep = 0;
 
    /*Levanto las paginas de este proceso para poder armar stack*/         
  //  levantaPaginas((PROCESO *)&procesos[i]);
  
    
    habilitarPagina(&procesos[i]);
    
    procesos[i].ESP = ArmaStackFrame (proceso, procesos[i].stackstart, Limpia);
     
     /*Bajo las paginas para evitar que cualquier otro proceso escriba 
    en la zona de este*/
   // bajarPaginas((PROCESO*)&procesos[i]);
   deshabilitarPagina(&procesos[i]);
   
    procesos[i].padre = pidActual;

    if (!enBackground && pidActual > 0) {
        proceso_t *proc = TraerProcesoPorPid (pidActual);
        /* bloqueado esperando que termine su hijo */
        proc->estado = ESPERANDO_HIJO;
        procesos[i].padre = pidActual;
    }
    procesos[i].free_slot = FALSE;
    
}

void sleep(int segundos)
{
    _Cli();
    long ticks = (long)(segundos * TIMER_TICK);    
    proceso_t * tmp = TraerProcesoPorPid(pidActual);
    tmp->sleep = ticks;
    tmp->estado = BLOQUEADO;

    switch_manual();
    _Sti();
}

void
Kill (int pid) {
    proceso_t * proc;
    proceso_t * padre;
    if (pid != INIT && pid !=1) {
        proc = (proceso_t*)TraerProcesoPorPid (pid);
        if (proc != 0) {
            printf ("El proceso ", 11);
            printf (proc->nombre, strlen (proc->nombre));
            printf (" fue eliminado.\n", 16);
            proc->free_slot = 1;
            proc->estado = 0;
            proc->pid = -1;
            MatarHijos (pid);
            if(proc->padre!=0)
            {
                padre = (proceso_t*)TraerProcesoPorPid (proc->padre);

                if(padre->padre != 0 && !proc->background )
                    desbloqueaProceso(proc->padre);
                else
                { 
                        if( ! proc->background )
                        {
                            desbloqueaProceso(proc->padre);
                            //bloqueaProceso (proc->padre, 1);
                        }
                }

            }
            else if (proc->padre != INIT)
                desbloqueaProceso(proc->padre);
        }
        else
            printf ("Proceso no encontrado.\n", 23);
    }
    else {
        printf ("Acceso denegado (lero lero).\n", 19);
    }
    return;
}

void
MatarHijos(int pid)
{
    int i;
    for(i = 1;i<MAXPROCESOS;i++)
    {
        if(procesos[i].padre == pid)
            Kill(procesos[i].pid);
    }
}

int
Nada (int argc, char **argv) {
    while (1)
    {
        asm volatile ("hlt");
    }
}

int
estaBloqueado(int pid)
{
  int i;

  for (i = 0; i < MAXPROCESOS; i++) {
    if (procesos[i].pid == pid && !procesos[i].free_slot) {
    
      if(procesos[i].estado !=BLOQUEADO && procesos[i].estado !=ESPERANDO_HIJO)
        return 1;
      else
        return 0;
    
    }
  }
  if (i == MAXPROCESOS)
    return -1;
}

int
bloqueaProceso (int pid, int bloqCode) {
  int i;

  for (i = 0; i < MAXPROCESOS; i++) {
    if (procesos[i].pid == pid && !procesos[i].free_slot) {
      procesos[i].estado = bloqCode;
      break;
    }
  }

  if (i == MAXPROCESOS)
    return -1;

  return 0;
    }

int
desbloqueaProceso (int pid) {
    int i;

    for (i = 0; i < MAXPROCESOS; i++) {
    if (procesos[i].pid == pid && !procesos[i].free_slot) {
      if(procesos[i].estado == ESPERANDO_HIJO || procesos[i].estado == BLOQUEADO)
            procesos[i].estado = LISTO;
        break;
    }
    }

    if (i == MAXPROCESOS)
    return -1;

    return 0;
}
