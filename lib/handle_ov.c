#include<printf.h>

extern char *KERNEL_SP;

void ov_do(void)
{
	struct Trapframe *tf;
	int i;
	tf = (struct Trapframp *)(KERNEL_SP - sizeof(struct Trapframe));
	u_int *instr_p = tf->cp0_epc;
	u_int instr = *instr_p
	printf("Instr: 0x%x\n",instr_p);
	printf("reg %d and reg %d\n", instr|(0x1f << 21), instr|(0x1f << 16));
	for (i = 8; i <= 15; i++)
	{
		tf->regs[i] = 0;
	}
	return;
}
