#ifndef _TRAP_H_
#define _TRAP_H_

/* these are processor defined */
#define T_DIVIDE     0    /* divide error */
#define T_DEBUG      1    /* debug exception */
#define T_NMI        2    /* non-maskable interrupt */
#define T_BRKPT      3    /* breakpoint */
#define T_OFLOW      4    /* overflow */
#define T_BOUND      5    /* bounds check */
#define T_ILLOP      6    /* illegal opcode */
#define T_DEVICE     7    /* device not available */
#define T_DBLFLT     8    /* double fault */
/* 9 is reserved */
#define T_TSS       10    /* invalid task switch segment */
#define T_SEGNP     11    /* segment not present */
#define T_STACK     12    /* stack exception */
#define T_GPFLT     13    /* genernal protection fault */
#define T_PGFLT     14    /* page fault */
/* 15 is reserved */
#define T_FPERR     16    /* floating point error */
#define T_ALIGN     17    /* aligment check */
#define T_MCHK      18    /* machine check */

/* These are arbitrarily chosen, but with care not to overlap
 * processor defined exceptions or interrupt vectors.
 */
#define T_SYSCALL   0x30 /* system call */
#define T_DEFAULT   500  /* catchall */

#ifndef __ASSEMBLER__

#include <types.h>

struct Trapframe { //lr:need to be modified(reference to linux pt_regs) TODO
	/* Saved main processor registers. */
	unsigned long regs[32];

	/* Saved special registers. */
	unsigned long cp0_status;
	unsigned long hi;
	unsigned long lo;
	unsigned long cp0_badvaddr;
	unsigned long cp0_cause;
	unsigned long cp0_epc;
	unsigned long pc;
};
void *set_except_vector(int n, void *addr);
void trap_init();

#endif /* !__ASSEMBLER__ */
/*
 * Stack layout for all exceptions:
 *
 * ptrace needs to have all regs on the stack. If the order here is changed,
 * it needs to be updated in include/asm-mips/ptrace.h
 *
 * The first PTRSIZE*5 bytes are argument save space for C subroutines.
 */

#define TF_REG0		0
#define TF_REG1		((TF_REG0) + 4)
#define TF_REG2		((TF_REG1) + 4)
#define TF_REG3		((TF_REG2) + 4)
#define TF_REG4		((TF_REG3) + 4)
#define TF_REG5		((TF_REG4) + 4)
#define TF_REG6		((TF_REG5) + 4)
#define TF_REG7		((TF_REG6) + 4)
#define TF_REG8		((TF_REG7) + 4)
#define TF_REG9		((TF_REG8) + 4)
#define TF_REG10	((TF_REG9) + 4)
#define TF_REG11	((TF_REG10) + 4)
#define TF_REG12	((TF_REG11) + 4)
#define TF_REG13	((TF_REG12) + 4)
#define TF_REG14	((TF_REG13) + 4)
#define TF_REG15	((TF_REG14) + 4)
#define TF_REG16	((TF_REG15) + 4)
#define TF_REG17	((TF_REG16) + 4)
#define TF_REG18	((TF_REG17) + 4)
#define TF_REG19	((TF_REG18) + 4)
#define TF_REG20	((TF_REG19) + 4)
#define TF_REG21	((TF_REG20) + 4)
#define TF_REG22	((TF_REG21) + 4)
#define TF_REG23	((TF_REG22) + 4)
#define TF_REG24	((TF_REG23) + 4)
#define TF_REG25	((TF_REG24) + 4)
/*
 * $26 (k0) and $27 (k1) not saved
 */
#define TF_REG26	((TF_REG25) + 4)
#define TF_REG27	((TF_REG26) + 4)
#define TF_REG28	((TF_REG27) + 4)
#define TF_REG29	((TF_REG28) + 4)
#define TF_REG30	((TF_REG29) + 4)
#define TF_REG31	((TF_REG30) + 4)

#define TF_STATUS	((TF_REG31) + 4)

#define TF_HI		((TF_STATUS) + 4)
#define TF_LO		((TF_HI) + 4)

#define TF_BADVADDR	((TF_LO)+4)
#define TF_CAUSE	((TF_BADVADDR) + 4)
#define TF_EPC		((TF_CAUSE) + 4)
#define TF_PC		((TF_EPC) + 4)
/*
 * Size of stack frame, word/double word alignment
 */
#define TF_SIZE		((TF_PC)+4)
#endif /* _TRAP_H_ */
