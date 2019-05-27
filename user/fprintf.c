#include "lib.h"

static int curlen;

static void user_out2string(void *arg, char *s, int l)
{
    int i;
	char * b = (char *)arg + curlen;
    // special termination call
    if ((l==1) && (s[0] == '\0')) return;
    
    for (i=0; i< l; i++) {
		b[i]=s[i];
    }
	curlen += l;
}


int fwritef(int fd, const char *fmt, ...)
{
	char buf[512];
	va_list ap;
	curlen = 0;
	va_start(ap, fmt);
	user_lp_Print(user_out2string, buf, fmt, ap);
	va_end(ap);
	buf[curlen] = '\0';
	return write(fd, buf, strlen(buf));
}
