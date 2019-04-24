#include "lib.h"



static void user_out2string(void *arg, const char *s, int l)
{
	int i;
	char *b = (char *)arg;

	// special termination call
	if ((l == 1) && (s[0] == '\0')) {
		return;
	}

	for (i = 0; i < l; i++) {
		b[i] = s[i];
	}
}


int fwritef(int fd, const char *fmt, ...)
{
	char buf[512];
	va_list ap;
	va_start(ap, fmt);
	user_bzero((void *)buf, 512);
	user_lp_Print(user_out2string, buf, fmt, ap);
	va_end(ap);
	return write(fd, buf, strlen(buf));
}
