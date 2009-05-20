#include "../include/programas.h"

int
Top(int argc, char ** argv)
{
    char c;
    int i;
    proceso_t * proc;
    int salgo = FALSE;
    while(!salgo)
    {
        _Sti();
        clear_screen();

        ActualizarPorcentajesCPU();
        printf("\t\t------------------ TOP ----------------------\n");
        printf("\t\t---------------------------------------------\n\n");
        printf("\t\t     PID         Command         CPU      \n");
        printf("\t\t---------------------------------------------\n\n");

        for(i = 0; i < MAXPROCESOS; i++)
        {
            proc = TraerProcesoPorIndice(i);
            if(!proc->free_slot)
            {
                printf("\t\t     %d            %s             %d%%\n",proc->pid, proc->nombre, proc->cpu );
            }
        }
        _Cli();
       // sleep(1);NumerosRandom
        int k,j;
        for(j = 0;j<31000;j++)
            for(k = 0; k<100;k++)
                ;

       if(!EstoyEnBackground())
       {
           if(!BufferIsEmpty())
               salgo = TRUE;
       }

    }
}


int
NumerosRandom(int argc, char ** argv)
{
    int i;
    int k,j;
    printf("\n\n\n\n\n\n\n\n\n");
    while(1)
    {
      /*  for(j = 0;j<31000;j++)
            for(k = 0; k<10;k++)
                ;*/
        i = random(100);
        for(j = 0;j<310;j++)
            ;
        printf("%d\n", i);
    }
}


/* TODO: permitirle que reciba por linea de comandos
*  cada cuantos segundos imprimir la letra 'a'
*/
int
ImprimeLetras(int argc, char ** argv)
{
  long i = 0, j = 0;
    //while(1)
    //{ 

        for(i = 0; i < 5000000; i++)
        {

	    printf("imprimiendo lalala\n");
	}
        sleep(25);
    //}
}

int
ImprimeLetras4Ever(int argc, char ** argv)
{
  long i = 0, j = 0;
    while(1)
    { 
        printf("\n\nasdfqwerty\n\n");

        sleep(50);
    }
}

