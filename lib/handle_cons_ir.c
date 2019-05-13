#include <printf.h>

void print_situation(int v)
{
	printf("%x\n",v);
	return;
}

void handle_cons_ir(char c, int status)
{
	static char buffer[70];
	static int len = 0;
	printf("cp0 status: %x\n", status);
	if (c != '\r')
	{
		buffer[len] = c;
		len++;
	}
	else
	{
		buffer[len] = '\0';
		printf("length: %d\n", len);
		printf("content: %s\n", buffer);
		len = 0;
	}
}
