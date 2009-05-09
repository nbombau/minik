#include "../include/excepciones.h"

void
Div0(void)
{

    printf("Error division por cero.\n");

} 

void
Bounds(void)
{

    printf("Bounds Error.\n");

}

void
opCode(void)
{

    printf("Opcode invalido.\n");

}

void
SnoPresent(void)
{

    printf("Segmento no presente.\n");

}

void
Stacksf(void)
{

    printf("Saturacion del segmento de pila.\n");

}

void    
GpFault(void)
{

    printf("General Protection Fault.\n");

}

void
PageFault(void)
{

    printf("Page Fault.\n");

}
