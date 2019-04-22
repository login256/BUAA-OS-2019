#include<printf.h>
#include<trap.h>

extern char *KERNEL_SP;

void ov_do(char* sp)
{
	struct Trapframe *tf;
	int i;
	tf = (struct Trapframp *)sp;// - sizeof(struct Trapframe));
	u_int *instr_p = tf->cp0_epc;
	u_int instr = *instr_p;
	printf("Instr: 0x%x\n",instr_p);
	printf("reg %d and reg %d\n", (instr>>21)&(0x1f), (instr>>16)&(0x1f));
	for (i = 8; i <= 15; i++)
	{
		tf->regs[i] = 0;
	}
	return;
}
