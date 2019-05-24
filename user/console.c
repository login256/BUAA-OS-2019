#include "lib.h"
#include <mmu.h>

static int cons_read(struct Fd*, void*, u_int, u_int);
static int cons_write(struct Fd*, const void*, u_int, u_int);
static int cons_close(struct Fd*);
static int cons_stat(struct Fd*, struct Stat*);

struct Dev devcons =
{
.dev_id=	'c',
.dev_name=	"cons",
.dev_read=	cons_read,
.dev_write=	cons_write,
.dev_close=	cons_close,
.dev_stat=	cons_stat,
};

int
iscons(int fdnum)
{
	int r;
	struct Fd *fd;

	if ((r = fd_lookup(fdnum, &fd)) < 0)
		return r;
	return fd->fd_dev_id == devcons.dev_id;
}

int
opencons(void)
{
	int r;
	struct Fd *fd;

	if ((r = fd_alloc(&fd)) < 0)
		return r;
	if ((r = syscall_mem_alloc(0, (u_int)fd, PTE_V|PTE_R|PTE_LIBRARY)) < 0)
		return r;
	fd->fd_dev_id = devcons.dev_id;
	fd->fd_omode = O_RDWR;
	return fd2num(fd);
}

int
cons_read(struct Fd *fd, void *vbuf, u_int n, u_int offset)
{
	int c;

	USED(offset);
//	printf("got into cons_read");
	if (n == 0)
		return 0;

	while ((c = syscall_cgetc()) == 0)
		syscall_yield();

	if (c!='\r') 
		writef("%c",c);
	else
		writef("\n");
	if (c < 0)
		return c;
	if (c == 0x04)	// ctl-d is eof
		return 0;
	*(char*)vbuf = c;
	return 1;
}

int
cons_write(struct Fd *fd, const void *vbuf, u_int n, u_int offset)
{
	int tot, m;
	char buf[128];

	USED(offset);

	// mistake: have to nul-terminate arg to syscall_cputs, 
	// so we have to copy vbuf into buf in chunks and nul-terminate.
	for(tot=0; tot<n; tot+=m) {
		m = n - tot;
		if (m > sizeof buf-1)
			m = sizeof buf-1;
		user_bcopy((char*)vbuf+tot, buf, m);
		buf[m] = 0;
		writef("%s",buf);
	}
	return tot;
}

int
cons_close(struct Fd *fd)
{
	USED(fd);

	return 0;
}

int
cons_stat(struct Fd *fd, struct Stat *stat)
{
	strcpy(stat->st_name, "<cons>");
	return 0;
}
