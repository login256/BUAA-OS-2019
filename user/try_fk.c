#include "lib.h"

void umain()
{
	int i;
	int child_id = 0;
	for (i = 1; i <= 62; i++)
	{
		if ((child_id = fork()) != 0)
		{
			break;
		}
	}
	writef("I am the No.%d\n", i);
	writef("My child's envid is %x\n", child_id);
}
