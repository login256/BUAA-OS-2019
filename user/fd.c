#include "lib.h"
#include "fd.h"
#include <mmu.h>
#include <env.h>

#define debug 0

#define MAXFD 32
#define FILEBASE 0x60000000
#define FDTABLE (FILEBASE-PDMAP)

#define INDEX2FD(i)	(FDTABLE+(i)*BY2PG)
#define INDEX2DATA(i)	(FILEBASE+(i)*PDMAP)

static struct Dev *devtab[] = {
	&devfile,
	&devcons,
	&devpipe,
	0
};

int
dev_lookup(int dev_id, struct Dev **dev)
{
	int i;

	//writef("dev_lookup()VVVVVVVVVVVVVVVVVVVVVV\n");
	for (i = 0; devtab[i]; i++)
		if (devtab[i]->dev_id == dev_id) {
			*dev = devtab[i];
			return 0;
		}

	writef("[%08x] unknown device type %d\n", env->env_id, dev_id);
	return -E_INVAL;
}

int
fd_alloc(struct Fd **fd)
{
	// Your code here.
	//
	// Find the smallest i from 0 to MAXFD-1 that doesn't have
	// its fd page mapped.  Set *fd to the fd page virtual address.
	// (Do not allocate a page.  It is up to the caller to allocate
	// the page.  This means that if someone calls fd_alloc twice
	// in a row without allocating the first page we return, we'll
	// return the same page the second time.)
	// Return 0 on success, or an error code on error.
	u_int va;
	u_int fdno;

	//writef("enter fd_alloc\n");
	for (fdno = 0; fdno < MAXFD - 1; fdno++) {
		va = INDEX2FD(fdno);

		if (((* vpd)[va / PDMAP] & PTE_V) == 0) {
			*fd = (struct Fd *)va;
			return 0;
		}

		//writef("fd_alloc:va = %x\n",va);
		if (((* vpt)[va / BY2PG] & PTE_V) == 0) {	//the fd is not used
			*fd = (struct Fd *)va;
			return 0;
		}
	}

	//user_panic("fd_alloc not implemented");
	return -E_MAX_OPEN;
}

void
fd_close(struct Fd *fd)
{
	syscall_mem_unmap(0, (u_int)fd);
}

int
fd_lookup(int fdnum, struct Fd **fd)
{
	// Your code here.
	//
	// Check that fdnum is in range and mapped.  If not, return -E_INVAL.
	// Set *fd to the fd page virtual address.  Return 0.
	u_int va;

	if (fdnum >= MAXFD) {
		return -E_INVAL;
	}

	va = INDEX2FD(fdnum);

	if (((* vpt)[va / BY2PG] & PTE_V) != 0) {	//the fd is used
		*fd = (struct Fd *)va;
		return 0;
	}

	//user_panic("fd_lookup not implemented");
	return -E_INVAL;
}

u_int
fd2data(struct Fd *fd)
{
	return INDEX2DATA(fd2num(fd));
}

int
fd2num(struct Fd *fd)
{
	return ((u_int)fd - FDTABLE) / BY2PG;
}
int
num2fd(int fd)
{
	return fd * BY2PG + FDTABLE;
}

int
close(int fdnum)
{
	int r;
	struct Dev *dev;
	struct Fd *fd;

	if ((r = fd_lookup(fdnum, &fd)) < 0
		||  (r = dev_lookup(fd->fd_dev_id, &dev)) < 0) {
		return r;
	}

	r = (*dev->dev_close)(fd);
	fd_close(fd);
	return r;
}

void
close_all(void)
{
	int i;

	for (i = 0; i < MAXFD; i++) {
		close(i);
	}
}

int
dup(int oldfdnum, int newfdnum)
{
	int i, r;
	u_int ova, nva, pte;
	struct Fd *oldfd, *newfd;

	//writef("dup comes 1;\n");
	if ((r = fd_lookup(oldfdnum, &oldfd)) < 0) {
		return r;
	}

	close(newfdnum);
	//writef("dup comes 2;\n");
	newfd = (struct Fd *)INDEX2FD(newfdnum);
	ova = fd2data(oldfd);
	nva = fd2data(newfd);

	if ((r = syscall_mem_map(0, (u_int)oldfd, 0, (u_int)newfd,
							 ((*vpt)[VPN(oldfd)]) & (PTE_V | PTE_R | PTE_LIBRARY))) < 0) {
		goto err;
	}

	//writef("dup comes 2.5;\n");
	if ((* vpd)[PDX(ova)]) {
		for (i = 0; i < PDMAP; i += BY2PG) {
			pte = (* vpt)[VPN(ova + i)];

			if (pte & PTE_V) {
				// should be no error here -- pd is already allocated
				if ((r = syscall_mem_map(0, ova + i, 0, nva + i,
										 pte & (PTE_V | PTE_R | PTE_LIBRARY))) < 0) {
					goto err;
				}
			}
		}
	}

	//writef("dup comes 3;\n");
	return newfdnum;

err:
	//writef("dup comes 4;\n");
	syscall_mem_unmap(0, (u_int)newfd);

	for (i = 0; i < PDMAP; i += BY2PG) {
		syscall_mem_unmap(0, nva + i);
	}

	return r;
}

int
read(int fdnum, void *buf, u_int n)
{
	int r;
	struct Dev *dev;
	struct Fd *fd;

	//writef("read() come 1 %x\n",(int)env);
	if ((r = fd_lookup(fdnum, &fd)) < 0
		||  (r = dev_lookup(fd->fd_dev_id, &dev)) < 0) {
		return r;
	}

	//writef("read() come 2 %x\n",(int)env);
	if ((fd->fd_omode & O_ACCMODE) == O_WRONLY) {
		writef("[%08x] read %d -- bad mode\n", env->env_id, fdnum);
		return -E_INVAL;
	}

	//writef("read() come 3 %x\n",(int)env);
	r = (*dev->dev_read)(fd, buf, n, fd->fd_offset);

	if (r >= 0) {
		fd->fd_offset += r;
	}

	//writef("read() come 4 %x\n",(int)env);
	return r;
}

int
readn(int fdnum, void *buf, u_int n)
{
	int m, tot;

	for (tot = 0; tot < n; tot += m) {
		m = read(fdnum, (char *)buf + tot, n - tot);

		if (m < 0) {
			return m;
		}

		if (m == 0) {
			break;
		}
	}

	return tot;
}

int
write(int fdnum, const void *buf, u_int n)
{
	int r;
	struct Dev *dev;
	struct Fd *fd;

	//writef("write comes 1\n");
	if ((r = fd_lookup(fdnum, &fd)) < 0
		||  (r = dev_lookup(fd->fd_dev_id, &dev)) < 0) {
		return r;
	}

	//writef("write comes 2\n");
	if ((fd->fd_omode & O_ACCMODE) == O_RDONLY) {
		writef("[%08x] write %d -- bad mode\n", env->env_id, fdnum);
		return -E_INVAL;
	}

	//writef("write comes 3\n");
	if (debug) writef("write %d %p %d via dev %s\n",
						  fdnum, buf, n, dev->dev_name);

	r = (*dev->dev_write)(fd, buf, n, fd->fd_offset);

	if (r > 0) {
		fd->fd_offset += r;
	}

	//writef("write comes 4\n");
	return r;
}

int
seek(int fdnum, u_int offset)
{
	int r;
	struct Fd *fd;

	//writef("seek() come 1 %x\n",(int)env);
	if ((r = fd_lookup(fdnum, &fd)) < 0) {
		return r;
	}

	//writef("seek() come 2 %x\n",(int)env);
	fd->fd_offset = offset;
	//writef("seek() come 3 %x\n",(int)env);
	return 0;
}


int fstat(int fdnum, struct Stat *stat)
{
	int r;
	struct Dev *dev;
	struct Fd *fd;

	if ((r = fd_lookup(fdnum, &fd)) < 0
		||  (r = dev_lookup(fd->fd_dev_id, &dev)) < 0) {
		return r;
	}

	stat->st_name[0] = 0;
	stat->st_size = 0;
	stat->st_isdir = 0;
	stat->st_dev = dev;
	return (*dev->dev_stat)(fd, stat);
}

int
stat(const char *path, struct Stat *stat)
{
	int fd, r;

	if ((fd = open(path, O_RDONLY)) < 0) {
		return fd;
	}

	r = fstat(fd, stat);
	close(fd);
	return r;
}

