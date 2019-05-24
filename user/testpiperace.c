#include "lib.h"

void
umain(void)
{
	int p[2], r, pid, i, max;
	u_int va;
	struct Fd *fd;
	struct Env *kid;

	writef("testing for dup race...\n");
	if ((r = pipe(p)) < 0)
		user_panic("pipe: %e", r);
	max = 200;
	if ((r = fork()) < 0)
		user_panic("fork: %e", r);
	if (r == 0) {
		close(p[1]);
		//
		// Now the ref count for p[0] will toggle between 2 and 3
		// as the parent dups and closes it (there's a close implicit in dup).
		// 
		// The ref count for p[1] is 1.
		// Thus the ref count for the underlying pipe structure 
		// will toggle between 3 and 4.
		//
		// If a clock interrupt catches close between unmapping
		// the pipe structure and unmapping the fd, we'll have
		// a ref count for p[0] of 3, a ref count for p[1] of 1,
		// and a ref count for the pipe structure of 3, which is 
		// a no-no.
		//
		// If a clock interrupt catches dup between mapping the
		// fd and mapping the pipe structure, we'll have the same
		// ref counts, still a no-no.
		//
		for (i=0; i<max; i++) {
			if(pipeisclosed(p[0])){
				writef("RACE: pipe appears closed\n");
				exit();
			}
			syscall_yield();
		}
		// do something to be not runnable besides exiting
		ipc_recv(0,0,0);
	}
	pid = r;
	writef("pid is %d\n", pid);
	va = 0;
	kid = &envs[ENVX(pid)];
	writef("kid is %d\n", kid-envs);
	dup(p[0], 10);
	while (kid->env_status == ENV_RUNNABLE)
		dup(p[0], 10);

	writef("child done with loop\n");
	if (pipeisclosed(p[0]))
		user_panic("somehow the other end of p[0] got closed!");
	if ((r = fd_lookup(p[0], &fd)) < 0)
		user_panic("cannot look up p[0]: %e", r);
	va = fd2data(fd);
	if (pageref((void*)va) != 3+1)
		writef("\nchild detected race\n");
	else
		writef("\nrace didn't happen\n", max);
}
