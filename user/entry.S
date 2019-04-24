#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>
#include <mmu.h>
#include <trap.h>
	.data

	.p2align 12

	.globl fsipcbuf
fsipcbuf:
	.space BY2PG
	.globl fdtab
fdtab:
	.space BY2PG


	.globl envs
envs:
	.word UENVS

	.globl pages
pages:
	.word UPAGES

	.globl vpt
vpt:
	.word UVPT

	.globl vpd
vpd:
	.word (UVPT+(UVPT>>12)*4)

	.globl __pgfault_handler
	__pgfault_handler:
	.word 0


	.extern libmain

	.text
	.globl _start
_start:
	//1: j 1b
	lw	a0, 0(sp)
	lw	a1, 4(sp)
	//sw	a0, -4(sp)
	//sw	a1, -0(sp)
nop
	jal	libmain
	nop

.set noreorder
.text
.globl __asm_pgfault_handler
__asm_pgfault_handler:
	// save the caller-save registers
	//	(your code here)
//1: j 1b
nop
	lw	a0, TF_BADVADDR(sp)
	//sw	t0, (sp)
	//subu	sp,16
	lw	t1, __pgfault_handler
	jalr	t1

nop
	// push trap-time eip, eflags onto trap-time stack
	//	(your code here)
	//addu	sp,16
	lw	v1,TF_LO(sp)                                       
		mtlo	v1                               
		lw	v0,TF_HI(sp)                                         
		lw	v1,TF_EPC(sp)                    
		mthi	v0                               
		mtc0	v1,CP0_EPC                                             
		lw	$31,TF_REG31(sp)                 
		lw	$30,TF_REG30(sp)                 
		lw	$28,TF_REG28(sp)                 
		lw	$25,TF_REG25(sp)                 
		lw	$24,TF_REG24(sp)                 
		lw	$23,TF_REG23(sp)                 
		lw	$22,TF_REG22(sp)                 
		lw	$21,TF_REG21(sp)                 
		lw	$20,TF_REG20(sp)                 
		lw	$19,TF_REG19(sp)                 
		lw	$18,TF_REG18(sp)                 
		lw	$17,TF_REG17(sp)                 
		lw	$16,TF_REG16(sp)                 
		lw	$15,TF_REG15(sp)                 
		lw	$14,TF_REG14(sp)                 
		lw	$13,TF_REG13(sp)                 
		lw	$12,TF_REG12(sp)                 
		lw	$11,TF_REG11(sp)                 
		lw	$10,TF_REG10(sp)                 
		lw	$9,TF_REG9(sp)                   
		lw	$8,TF_REG8(sp)                   
		lw	$7,TF_REG7(sp)                   
		lw	$6,TF_REG6(sp)                   
		lw	$5,TF_REG5(sp)                   
		lw	$4,TF_REG4(sp)                   
		lw	$3,TF_REG3(sp)                   
		lw	$2,TF_REG2(sp)                   
		lw	$1,TF_REG1(sp) 
		lw	k0,TF_EPC(sp) 	//atomic operation needed 
		jr	k0			//
		lw	sp,TF_REG29(sp)  /* Deallocate stack */  

