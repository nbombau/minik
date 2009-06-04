#include "../include/programas.h"

extern int mem[MAX_PAGES];
int
Top(int argc, char ** argv)
{
    int i;
    proceso_t * proc;
    int salgo = FALSE;
    while(!salgo)
    {
        //_Sti();
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
        //_Cli();
        sleep(1);
	
	if(!EstoyEnBackground())
	{
	    if(!BufferIsEmpty())
		salgo = TRUE;
	}
	
    }
    return 0;
}


int
NumerosRandom(int argc, char ** argv)
{
    int i;
    int j;
    printf("\n\n\n\n\n\n\n\n\n");
    while(1)
    {
        i = random(100);
        for(j = 0;j<310;j++)
            ;
        printf("%d\n", i);
    }
    return 0;
}


/* TODO: permitirle que reciba por linea de comandos
*  cada cuantos segundos imprimir la letra 'a'
*/
int
ImprimeLetras(int argc, char ** argv)
{
    long i = 0;
    for(i = 0; i < 10; i++)
    {

	printf("imprimiendo lalala\n");
	sleep(3);
    }
    return 0;
}

int
ImprimeLetras4Ever(int argc, char ** argv)
{
    while(1)
    { 
        printf("\n\nasdfqwerty\n\n");

        sleep(10);
    }
    return 0;
}

int
Vacio(int argc, char ** argv)
{
    while(1)
	sleep(100);

    return 0;
}
int
Calcula(int argc, char ** argv)
{
    int i, j,k;
    for(i = 0;i<31000;i++)
        for(j = 0;j<31000;j++)
            for(k = 0;k<31000;k++)
                ; 
    return 0;
}


/*Para probar si anda el realloc del stack.*/
int
Recursivo(int argc,char**argv)
{
    int i, j;
    for(i = 0;i<31;i++)
        for(j = 0;j<31;j++)
            ;
    return Recursivo(argc,argv);
}

/*El clasico.*/
int 
Fortune(int argc, char **argv)
{

    static char fortunes[3][600] = {
        "\nSon las 4 de la maniana y yo programando este fortune...\n",
        "\nVer un mundo en un grano de arena\ny el cielo en una flor silvestre\ncontener el infinito en la palma de la mano\ny la eternidad en una hora\n\n\t William Blake",
        "\nRTFM\n\n\t Fernando Zunino y muchos mas"
    };
    printf("%s\n", fortunes[random(3)]);
    return 0;
}

/* Testeo de Exception Handlers
*/
int 
DividirPorCero(int argc, char **argv)
{
    int i = 0,j;
    j = NUMERO_PRIMO/i;
    return 0;
}

int
PageFaultTest(int argc, char **argv)
{
    char * a = (char *) 0xF00000;
    int i = 0;
    while( i < 99999)
	a[i++] = 'a';
    return 0;
}

int
Paginas(int argc, char **argv)
{
    int i;
    for(i=0;i<35;i++)
    {
	printf("(%d)*",mem[i]);
    }
    printf("(%d)\n\n\n",mem[i]);
    return 0;
}

int
LlenarStack(int argc, char **argv)
{
    _StackFill();
    return 0;
}

