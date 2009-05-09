#include "../include/defs.h"
#include "../include/kc.h"


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
        len2 = strlen (procesos[i].nombre);

        if (strncmp (proceso, procesos[i].nombre, len1) == 0 && len1 == len2
            && !procesos[i].free_slot) {
            return &procesos[i];
        }
    }

    return 0;
}

int
NoHayProcesos (void) {
    int i;

    for (i = 0; i < MAXPROCESOS; i++) {
        if (i == INIT)
            continue;
        if (procesos[i].free_slot == 0 && !procesos[i].estado)
            return 0;
    }
    return 1;
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

  temporal = TraerProcesoPorPid (pidActual);

  if (temporal->padre != INIT) {
    padre = TraerProcesoPorPid (temporal->padre);
    padre->estado = 0;
  }
  temporal->free_slot = 1;
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

 
    //stack = (void *)Malloc (stacksize);
    stack = (void *)(0xF00000 + (veces * 1024)) ;
    for (i = 1; i < MAXPROCESOS; i++) {


        if (procesos[i].free_slot)
            break;
    }


    procesos[i].pid = NuevoPid ();
    procesos[i].background = enBackground;
    procesos[i].prioridad = prioridad;
    strncpy (procesos[i].nombre, nombre, strlen (nombre)+1);
    procesos[i].estado = LISTO;
    procesos[i].stacksize = stacksize;
    procesos[i].stackstart = (int) stack + stacksize - 1;
 //   procesos[i].nextfree = (int) heap;
    
    /*Levanto las paginas de este proceso para poder armar stack*/         
  //  levantaPaginas((PROCESO *)&procesos[i]);
    
    procesos[i].ESP = ArmaStackFrame (proceso, procesos[i].stackstart, Limpia);

     /*Bajo las paginas para evitar que cualquier otro proceso escriba 
    en la zona de este*/
   // bajarPaginas((PROCESO*)&procesos[i]);
    
    procesos[i].padre = pidActual;

    if (!enBackground && pidActual > 0) {
        proceso_t *proc = TraerProcesoPorPid (pidActual);
        /* bloqueado esperando que termine su hijo */
        proc->estado = ESPERANDO_HIJO;
       // procesos[i].padre = pidActual;
    }
    procesos[i].free_slot = 0;  
  
}

void
    Kill (int pid) {/*
    PROCESO * proc;
    PROCESO * padre;
    if (pid != GOD) {
        proc = GetProcessByPid (pid);
        if (proc != 0) {
            printf ("El proceso ", 11);
            printf (proc->nombre, strlen (proc->nombre));
            printf (" fue eliminado.\n", 16);
            proc->free_slot = 1;
            proc->bloqueado = 0;
            proc->pid = -1;
            matarHijos (pid);
            if(proc->padre!=0)
            {
                padre = GetProcessByPid (proc->padre);
                if (proc->padre != GOD )
                {
                    if(padre->padre != 0 && !proc->background )
                        desbloqueaProceso(proc->padre);
                    else if(padre->terminal==terminalActual && !proc->background )
                        desbloqueaProceso(proc->padre);
                    else
                    { 
                           if( ! proc->background )
                   {
                desbloqueaProceso(proc->padre);
                            bloqueaProceso (proc->padre, 1);
               }
                    }
                }
            }
            else if (proc->padre != GOD)
                desbloqueaProceso(proc->padre);
        }
        else
            printf ("Proceso no encontrado.\n", 23);
    }
    else {
        printf ("Proceso protegido.\n", 19);
    }
    return;*/
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
