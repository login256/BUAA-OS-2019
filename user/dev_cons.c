#include "lib.h"

static void user_myoutput(void *arg, const char *s, int l)
{
	int i;

	// special termination call
	if ((l == 1) && (s[0] == '\0')) {
		return;
	}

	for (i = 0; i < l; i++) {
		syscall_write_dev(&s[i], 0x10000000 + 0x0, 1);

		if (s[i] == '\n') {
			syscall_write_dev(&s[i], 0x10000000 + 0x0, 1);
		}
	}
}

void uwritef(char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	user_lp_Print(user_myoutput, 0, fmt, ap);
	va_end(ap);
}

char ugetc()
{
	char c = 0;
	while (c == 0)
	{
		c = syscall_read_dev(&c, 0x10000000 + 0x0, 1);
	}
	return c;
}
