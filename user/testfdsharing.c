#include "lib.h"

char buf[512], buf2[512];

int 
memcmp(char *a, char *b, int n)
{
	int i;

	for(i=0; i<n; i++)
		if(a[i] != b[i])
			return 1;
	return 0;
}

void
umain(int argc, char **argv)
{
	int fd, r, n, n2;

	if ((fd = open("motd", O_RDONLY)) < 0)
		user_panic("open motd: %e", fd);
	seek(fd, 0);
	if ((n = readn(fd, buf, sizeof buf)) <= 0)
		user_panic("readn: %e", n);

	if ((r = fork()) < 0)
		user_panic("fork: %e", r);
	if (r == 0) {
		seek(fd, 0);
		writef("going to read in child (might page fault if your sharing is buggy)\n");
		if ((n2 = readn(fd, buf2, sizeof buf2)) != n2)
			user_panic("read in parent got %d, read in child got %d", n, n2);
		if(memcmp(buf, buf2, n) != 0)
			user_panic("read in parent got different bytes from read in child");
		writef("read in child succeeded\n");
		seek(fd, 0);
		close(fd);
		exit();
	}
	wait(r);
	//seek(fd, 0);
	if ((n2 = readn(fd, buf2, sizeof buf2)) != n)
		user_panic("read in parent got %d, then got %d", n, n2);
	writef("buf : %s\n",buf);
	writef("read in parent succeeded\n");		
}

