#include "lib.h"

int a[100];

void umain()
{
	int i = 0;
	a[2] = 10;
	if (tfork() == 0)
	{
		for (i = 0; i <= 100000; i++)
		{

		}
	}
	else
	{
		a[3] = 20;
	}
	writef("%d %d %d\n", a[2], a[3], i); 
}
