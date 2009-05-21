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
        sleep(1);
        int k,j;

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

    for(i = 0; i < 10; i++)
    {

	printf("imprimiendo lalala\n");
	sleep(25);
    }
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

int
Calcula(int argc, char ** argv)
{
    int i, j,k;
    for(i = 0;i<31000;i++)
        for(j = 0;j<31000;j++)
            for(k = 0;k<31000;k++)
                ; 
}

/*Para probar si anda el realloc del stack.*/
int
Recursivo(int argc,char**argv)
{
    int i, j;
    for(i = 0;i<31;i++)
        for(j = 0;j<31;j++)
            ;
    return Recursivo(argc, argv);
}

/*El clasico.*/
int 
Fortune(int argc, char **argv)
{
    int randNum;

    static char fortunes[3][600] = {
        "\nSon las 4 de la maniana y yo programando este fortune...\n",
        "\nVer un mundo en un grano de arena\ny el cielo en una flor silvestre\ncontener el infinito en la palma de la mano\ny la eternidad en una hora\n\n\t William Blake",
        "\nRTFM\n\n\t Fernando Zunino y muchos mas"
    };
    printf("%s\n", fortunes[random(3)]);
}

/* Testeo de Exception Handlers
*/
int 
DividirPorCero(int argc, char **argv)
{
    int i = 0,j;
    j = NUMERO_PRIMO/i;
}

int
PageFaultTest(int argc, char **argv)
{
    char * a = (char *) 0xF00000;
    int i = 0;
    while( i < 99999)
	a[i++] = 'a';

}
