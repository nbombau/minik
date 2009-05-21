#include "../include/excepciones.h"
#include "../include/defs.h"
#include "../include/kc.h"

extern int pidActual;

void
Div0(void)
{
    _Cli();
    printf("Exception: Division by Zero.\n");
    _debug();
    if(pidActual != INIT)
    {
        Kill(pidActual);
    }
    _Sti();
}

void
Bounds(void)
{
    _Cli();
    printf("Exception: Bounds Error.\n");
    _debug();
    if(pidActual != INIT)
    {
        Kill(pidActual);
    }
    _Sti();
}

void
opCode(void)
{
    _Cli();
    printf("Exception: Invalid Operation Code.\n");
    _debug();
    if(pidActual != INIT)
    {
        Kill(pidActual);
    }
    _Sti();
}

void
SnoPresent(void)
{
    _Cli();
    printf("Exception: Segment Not Present.\n");
    _debug();
    if(pidActual != INIT)
    {
        Kill(pidActual);
    }
    _Sti();
}

void
Stacksf(void)
{
    _Cli();
    printf("Exception: Stack Segment Overhead.\n");
    _debug();
    if(pidActual != INIT)
    {
        Kill(pidActual);
    }
    _Sti();
}

void
GpFault(void)
{
    _Cli();
    printf("Exception: General Protection Fault.\n");
    _debug();
    if(pidActual != INIT)
    {
        Kill(pidActual);
    }
    _Sti();
}

void
PageFault(void)
{
    
    printf("Exception: Page Fault.\n");
    _debug();
    if(pidActual != INIT)
    {
        Kill(pidActual);
    }
    _Sti();
}
