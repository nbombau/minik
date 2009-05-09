#include"../include/buffer.h"
#include "../include/defs.h"

static unsigned char queue[MAX_CANT+1];
static int head;
static int tail;

int
BufferIsEmpty(void)
{
	return ( head == tail );
}

int
BufferIsFull(void)
{
	return (tail + 1) % BUFFER_ARRAY_SIZE == head;
}

int
AddToBuffer(unsigned char c)
{
	if(  BufferIsFull() )
		return BUFFER_FULL;

	queue[tail] = c;
	tail = (tail + 1) % BUFFER_ARRAY_SIZE;

	return 0;
}

unsigned char
GetFromBuffer(void)
{
	unsigned char c;

	if(!BufferIsEmpty())
	{
		c = queue[head];
		head = (head + 1) % BUFFER_ARRAY_SIZE;
	}

	return c;
}

