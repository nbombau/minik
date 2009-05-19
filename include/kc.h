/**********************
 kc.h
**********************/
#include "defs.h"
#include "stdio.h"
#ifndef _kc_
#define _kc_

/* Inicializa la entrada del IDT */
void setup_IDT_entry (DESCR_INT *item, byte selector, dword offset, byte access,
			 byte cero);
void setup_GDT_entry (DESCR_SEG* item, dword base, dword limit, byte access, byte attribs);

int probarMemoria(int argc, char **argv);

int ProcesoExiste(int pid);

proceso_t * TraerProcesoPorPid(int pid);

proceso_t * TraerProcesoPorIndice(int i);

proceso_t *  TraerProcesoPorNombre(char *proceso) ;

int NoHayProcesos (void) ;

int NuevoPid (void) ;

void exec (char *nombre, int (*proceso) (int argc, char **argv), int enBackground) ;

void CrearProceso (char *nombre, int (*proceso) (int argc, char **argv), int argc, 
                   char **argv, int prioridad, int enBackground, int stacksize);   

int Halt (int argc, char **argv);

int EstaBloqueado(int pid);

int BloqueaProceso (int pid, int bloqCode);
    
int DesbloqueaProceso (int pid);

void Kill (int pid);

int Init(int argc, char ** argv);


    
void GuardarESP (int ESP) ;
int SiguienteProceso (int esp);
proceso_t * SiguienteTarea (void) ;

int CargarESP (proceso_t * proc) ;

void ActualizarPorcentajesCPU(void);
int CantidadDeProcesos(void);


void ReiniciarPorcentajes(void);
void IniciarMultiTarea (void) ;
int EstoyEnBackground();
int TraerIndiceProceso(int pid);

void KFree(int nPagina, int cantPaginas);
void * KRealloc(proceso_t * proc, int cantPaginas);
void * KMalloc (proceso_t * proc);
#endif
