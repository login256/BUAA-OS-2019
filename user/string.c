// String routines.  Not hardware optimized, but not shabby.

#include "lib.h"

int
strlen(const char *s)
{
	int n;

	for (n=0; *s; s++)
		n++;
	return n;
}

char*
strcpy(char *dst, const char *src)
{
	char *ret;

	ret = dst;
	while ((*dst++ = *src++) != 0)
		;
	return ret;
}

const char*
strchr(const char *s, char c)
{
	for(; *s; s++)
		if(*s == c)
			return s;
	return 0;
}

void *
memcpy(void *destaddr,void const *srcaddr,u_int len)
{
	char *dest = destaddr;
	char const *src = srcaddr;

	while(len-->0)
		*dest++=*src++;
	return destaddr;
}


int
strcmp(const char *p, const char *q)
{
	while (*p && *p == *q)
		p++, q++;
	if ((u_int)*p < (u_int)*q)
		return -1;
	if ((u_int)*p > (u_int)*q)
		return 1;
	return 0;
}

