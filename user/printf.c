

#include "lib.h"




void halt(void);

static void user_myoutput(void *arg, char *s, int l)
{
    int i;

    // special termination call
    if ((l==1) && (s[0] == '\0')) return;
    
    for (i=0; i< l; i++) {
	syscall_putchar(s[i]);
	if (s[i] == '\n') syscall_putchar('\n');
    }
}

void writef(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    user_lp_Print(user_myoutput, 0, fmt, ap);
    va_end(ap);
}

void
_user_panic(const char *file, int line, const char *fmt,...)
{
	va_list ap;


	va_start(ap, fmt);
	writef("panic at %s:%d: ", file, line);
	user_lp_Print(user_myoutput, 0, (char *)fmt, ap);
	writef("\n");
	va_end(ap);


	for(;;);
}
