#include "../include/excepciones.h"
#include "../include/defs.h"
#include "../include/kc.h"

extern int pidActual;

void
Div0(void)
{
    printf("Exception: Division by Zero.\n");
    if(pidActual != INIT)
    {
        Kill(pidActual);
    }
}

void
Bounds(void)
{
    printf("Exception: Bounds Error.\n");
    if(pidActual != INIT)
    {
        Kill(pidActual);
    }
}

void
opCode(void)
{
    printf("Exception: Invalid Operation Code.\n");
    if(pidActual != INIT)
    {
        Kill(pidActual);
    }
}

void
SnoPresent(void)
{
    printf("Exception: Segment Not Present.\n");
    if(pidActual != INIT)
    {
        Kill(pidActual);
    }
}

void
Stacksf(void)
{
    printf("Exception: Stack Segment Overhead.\n");
    if(pidActual != INIT)
    {
        Kill(pidActual);
    }
}

void
GpFault(void)
{
    printf("Exception: General Protection Fault.\n");
    if(pidActual != INIT)
    {
        Kill(pidActual);
    }
}

void
PageFault(void)
{
    printf("Exception: Page Fault.\n");
    if(pidActual != INIT)
    {
        Kill(pidActual);
    }
}
