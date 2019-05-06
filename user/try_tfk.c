#include "lib.h"

int a[100];

void umain()
{
	int i = 0;
	int nowid;
	int rc;
	a[2] = 10;
	if ((nowid = tfork()) == 0)
	{
		for (i = 0; i <= 100000; i++)
		{

		}
		rc = ipc_recv(&nowid, 0, 0);
		ipc_send(nowid, 666, 0, 0);
		writef("send to %x\n", nowid);
	}
	else
	{
		a[3] = 20;
		ipc_send(nowid, 233, 0, 0);
		writef("send to %x\n", nowid);
		rc = ipc_recv(&nowid, 0, 0);
		writef("%x\n",env->env_id);
	}
	writef("%X %x %d %d %d %d\n", syscall_getenvid(), nowid, a[2], a[3], rc, i); 
}
