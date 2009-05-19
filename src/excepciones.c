#include "../include/excepciones.h"

void
Div0(void)
{
    printf("Exception: Division by Zero.\n");
} 

void
Bounds(void)
{

    printf("Exception: Bounds Error.\n");

}

void
opCode(void)
{

    printf("Exception: Invalid Operation Code.\n");

}

void
SnoPresent(void)
{

    printf("Exception: Segment No Present.\n");

}

void
Stacksf(void)
{

    printf("Exception: Stack Segment Overhead.\n");

}

void
GpFault(void)
{

    printf("Exception: General Protection Fault.\n");

}

void
PageFault(void)
{

    printf("Exception: Page Fault.\n");

}
