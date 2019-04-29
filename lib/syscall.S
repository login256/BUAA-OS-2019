#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>
#include <stackframe.h>
#include <unistd.h>

NESTED(handle_sys,TF_SIZE, sp)
    SAVE_ALL                            // Macro used to save trapframe
    CLI                                 // Clean Interrupt Mask
    nop
    .set at                             // Resume use of $at

    // TODO: Fetch EPC from Trapframe, calculate a proper value and store it back to trapframe.

	lw		t0, TF_EPC(sp)
	lw		t1, TF_CAUSE(sp)
	lui		t2, 0x8000
	and		t1, t1, t2
	bnez	t1, IS_BD
	nop
	addiu	t0, t0, 4
	j BD_IF_END
	nop
IS_BD:
BD_IF_END:
	sw		t0, TF_EPC(sp)

    // TODO: Copy the syscall number into $a0.

	lw		a0, TF_REG4(sp)
    
    addiu   a0, a0, -__SYSCALL_BASE     // a0 <- relative syscall number
    sll     t0, a0, 2                   // t0 <- relative syscall number times 4
    la      t1, sys_call_table          // t1 <- syscall table base
    addu    t1, t1, t0                  // t1 <- table entry of specific syscall
    lw      t2, 0(t1)                   // t2 <- function entry of specific syscall

    lw      t0, TF_REG29(sp)            // t0 <- user's stack pointer

    // TODO: Allocate a space of six arguments on current kernel stack and copy the six arguments to proper location
    
	lw		a0, TF_REG4(sp)
	lw		a1, TF_REG6(sp)
	lw		a2, TF_REG7(sp)
	
	lw		s1, TF_REG5(sp)

	addiu	t4, s1, 1
	sll		s2, t4, 2
	subu	sp, sp, s2

	addiu	t0, t0, 16

	li		t3, 3
	blt		s1, t3, IF_1_END
	nop
		lw		a3, 0(t0)
		addiu	t0, t0, 4
	IF_1_END:

	li		t3, 4
	addiu	t5, sp, 16

	FOR_1_BEGIN:
	bgt		t3, s1, FOR_1_END
	nop
		lw		t4, 0(t0)
		addiu	t0, t0, 4
		sw		t4, 0(t5)
		addiu	t5, t5, 4
	addiu	t3, t3, 1
	j FOR_1_BEGIN
	nop
	FOR_1_END:
    
    jalr    t2                          // Invoke sys_* function
    nop
    
    // TODO: Resume current kernel stack
    
	addu	sp, sp, s2

    sw      v0, TF_REG2(sp)             // Store return value of function sys_* (in $v0) into trapframe

    j       ret_from_exception          // Return from exeception
    nop
END(handle_sys)

sys_call_table:                         // Syscall Table
.align 2
    .word sys_putchar
    .word sys_getenvid
    .word sys_yield
    .word sys_env_destroy
    .word sys_set_pgfault_handler
    .word sys_mem_alloc
    .word sys_mem_map
    .word sys_mem_unmap
    .word sys_env_alloc
    .word sys_set_env_status
    .word sys_set_trapframe
    .word sys_panic
    .word sys_ipc_can_send
    .word sys_ipc_recv
    .word sys_cgetc
