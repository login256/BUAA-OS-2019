#include "../drivers/gxconsole/dev_cons.h"
#include <mmu.h>
#include <env.h>
#include <printf.h>
#include <pmap.h>
#include <sched.h>

extern char *KERNEL_SP;
extern struct Env *curenv;


void sys_putchar(int sysno, int c, int a2, int a3, int a4, int a5)
{
	printcharc((char) c);
	return ;
}


void *memcpy(void *destaddr, void const *srcaddr, u_int len)
{
	char *dest = destaddr;
	char const *src = srcaddr;

	while (len-- > 0) {
		*dest++ = *src++;
	}

	return destaddr;
}

u_int sys_getenvid(void)
{
	return curenv->env_id;
}

void sys_yield(void)
{

}

int sys_env_destroy(int sysno, u_int envid)
{
    return 0;
}

int sys_set_pgfault_handler(int sysno, u_int envid, u_int func, u_int xstacktop)
{
    return 0;
}


int sys_mem_alloc(int sysno, u_int envid, u_int va, u_int perm)
{
    return 0;
}


int sys_mem_map(int sysno, u_int srcid, u_int srcva, u_int dstid, u_int dstva,
				u_int perm)
{
    return 0;
}

int sys_mem_unmap(int sysno, u_int envid, u_int va)
{
    return 0;
}

int sys_env_alloc(void)
{
    return 0;
}


int sys_set_env_status(int sysno, u_int envid, u_int status)
{
    return 0;
}

int sys_set_trapframe(int sysno, u_int envid, struct Trapframe *tf)
{
    return 0;
}

void sys_panic(int sysno, char *msg)
{

}

void sys_ipc_recv(int sysno, u_int dstva)
{

}

int sys_ipc_can_send(int sysno, u_int envid, u_int value, u_int srcva, u_int perm)
{
    return 0;
}
