#include "lib.h"

void umain()
{
	int a = 1,b = 2;
	a=ugetc();
	b=ugetc();
	uwritef("%d %d %d\n", a, b, a+b);
}
