#include "../include/defs.h"
#include "../include/kc.h"

#define CUANTUM_MAX 6

extern proceso_t procesos[];
extern int pidActual;
extern  int maxmem;

int indice = 0;
int ultimos100[100] = { 0 };
int nivelActual = 0;
int actual = 0;
int vuelta = 0;

//extern unsigned long *page_table;
//extern unsigned long *page_directory;

void
GuardarESP (int ESP) {

    proceso_t *temporal;
    temporal = TraerProcesoPorPid(pidActual);
    temporal->ESP = ESP;
    return;
}
/*
int
bajarPaginasAnterior (int pid) {
    PROCESO *myProcess;
    int io, il,j,i;


   if( pid == 0 )
     return 0;

    myProcess = GetProcessByPid (pid);

    if( pid == TEMP )
{
    bajarPaginas(NULL);
}
    
    bajarPaginas(myProcess);
    
    return 0;

}

int
levantaPaginasNuevo (int pid) {


    PROCESO *myProcess;
    int io, il,j,i;
     

    if( pid == 0 )
     return 0;

    myProcess = GetProcessByPid (pid);
       
       /* Temp es de temporal, se usa por lo pronto para levantar gran parte de las paginas ya que en creaProceso se llama a malloc y este esta tocando la memoria de 
       alguna manera, ergo explota si no estan levantadas */
 /*      
    if( pid == TEMP  )
{
        levantaPaginas(NULL);
}
     
     levantaPaginas(myProcess);
   
    return 0;
}



int bajarPaginas(PROCESO * myProcess)
{
    int io, il,j,i;
     
      if( myProcess == NULL  )
{
       io = INITIAL_PAGE;
       il = maxmem/4; // Da la ultima pagina a levantar
}
     else{
      
       io =  (int) (myProcess->heapstart -
                               (myProcess->stacksize +
                                myProcess->heapsize)) / 4096;
       il =  (int) myProcess->heapstart / 4096;
       
}  
     
   

   /* La posicion de la pagina dividido 1024 nos da la posicion en la tabla
de directorio de la tabla de paginas a la cual pertenece esa pagina */
 /*   for( i = io/1024; i <= il/1024 ; i++)
{
      int upTo;
      
      page_table = page_directory + 4096 * (i+1);
     
      if( i != io/1024 )
          j = 0;
      else 
          j = io%1024;
           
     
      if( i == il/1024) 
       upTo = (il-io)%1024+io%1024;
      else upTo = 1024;
       
       
       for(; j <  upTo ; j++)
         page_table[j] = page_table[j] & 0xFFFFFFFE;
    
       page_directory[i] = (unsigned long)page_table;
       page_directory[i] = page_directory[i] & 0xFFFFFFFE;;

} 
    
    
    return 0;
   
}


int
levantaPaginas(PROCESO * myProcess) {

    int io, il,j,i;
   
   
     
   if( myProcess == NULL  )
{
       io = INITIAL_PAGE;
       il = maxmem/4; // Da la ultima pagina a levantar
}
     else{
      
       io =  (int) (myProcess->heapstart -
                               (myProcess->stacksize +
                                myProcess->heapsize)) / 4096;
       il =  (int) myProcess->heapstart / 4096;
       
}  
     
   

   /* La posicion de la pagina dividido 1024 nos da la posicion en la tabla
de directorio de la tabla de paginas a la cual pertenece esa pagina */
  /*  for( i = io/1024; i <= il/1024 ; i++)
{
      int upTo;
      
      page_table = page_directory + 4096 * (i+1);
     
      if( i != io/1024 )
          j = 0;
      else 
          j = io%1024;
           
     
      if( i == il/1024) 
       upTo = (il-io)%1024+io%1024;
      else upTo = 1024;
       
       
       for(; j <  upTo ; j++)
         page_table[j] = page_table[j] | 7;
    
       page_directory[i] = (unsigned long)page_table;
       page_directory[i] = page_directory[i] | 7;

} 
    
    
    return 0;
}

*/

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
int yaPase=0;
int primero=1;
int elPrimero=0;
/* la papota */

proceso_t *
SiguienteTarea (void) {
    int i;
    int tengoProceso = 0;
    int cant = 0;

 /*   for (i = 0; i < MAXPROCESOS; i++) {
        if (peticionesDormir[i].time != 0) {
            if (--peticionesDormir[i].time == 0)
                desbloqueaProceso (peticionesDormir[i].pid);
}
}*/
    if (NoHayProcesos ()) {
        ultimos100[indice] = INIT;
        indice = (indice + 1) % 100;
        return &procesos[INIT];
}

    i = ((actual + 1) % MAXPROCESOS < 1) ? 1 : (actual + 1) % MAXPROCESOS;
    
    while (!tengoProceso) {
      if (!procesos[i].free_slot) {
                actual = i;
                ultimos100[indice] = i;
                indice = (indice + 1) % 100;
                return &procesos[i];
        }  
        cant++;
        i = (i + 1) % MAXPROCESOS < 1 ? 1 : (i + 1) % MAXPROCESOS;
    }
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
        procesos[i].free_slot = 1;

    /* no hay procesos en la lista de sleep 
    for (i = 0; i < MAXPROCESOS; i++)
    peticionesDormir[i].time = 0;*/

}
