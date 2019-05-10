#include"lib.h"

extern char end[];
void umain()
{
	int a = end;
	writef("%x\n",a);
}
