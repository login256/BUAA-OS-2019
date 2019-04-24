/*
 * Copyright (C) 2001 MontaVista Software Inc.
 * Author: Jun Sun, jsun@mvista.com or jsun@junsun.net
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include <printf.h>
#include <print.h>
#include <drivers/gxconsole/dev_cons.h>

static void myoutput(void *arg, char *s, int l)
{
	int i;

	// special termination call
	if ((l == 1) && (s[0] == '\0')) {
		return;
	}

	for (i = 0; i < l; i++) {
		printcharc(s[i]);

		if (s[i] == '\n') {
			printcharc('\n');
		}
	}
}

void printf(char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	lp_Print(myoutput, 0, fmt, ap);
	va_end(ap);
}

void
_panic(const char *file, int line, const char *fmt, ...)
{
	va_list ap;


	va_start(ap, fmt);
	printf("panic at %s:%d: ", file, line);
	lp_Print(myoutput, 0, (char *)fmt, ap);
	printf("\n");
	va_end(ap);


	for (;;);
}
