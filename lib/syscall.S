#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>
#include <stackframe.h>
#include <unistd.h>


NESTED(handle_sys,TF_SIZE, sp)

SAVE_ALL
CLI

//1: j 1b
nop
.set at
lw t1, TF_EPC(sp)
sw      t1, TF_EPC(sp)
la      t1, sys_call_table
lw      t2, (t1)
lw      t0,TF_REG29(sp)

lw      t1, (t0)
lw      t3, 4(t0)
lw      t4, 8(t0)
lw      t5, 12(t0)
lw      t6, 16(t0)
lw      t7, 20(t0)

subu    sp, 20

sw      t1, 0(sp)
sw      t3, 4(sp)
sw      t4, 8(sp)
sw      t5, 12(sp)
sw      t6, 16(sp)
sw      t7, 20(sp)

move    a0, t1
move    a1, t3
move    a2, t4
move    a3, t5

jalr    t2
nop

addu    sp, 20

sw      v0, TF_REG2(sp)

j       ret_from_exception//extern?
nop

illegal_syscall: j illegal_syscall
			nop
END(handle_sys)

	.extern sys_putchar
	.extern sys_getenvid
	.extern sys_yield
	.extern sys_env_destroy
	.extern sys_set_pgfault_handler
	.extern sys_mem_alloc
	.extern sys_mem_map
	.extern sys_mem_unmap
	.extern sys_env_alloc
	.extern sys_set_env_status
	.extern sys_set_trapframe
	.extern sys_panic
	.extern sys_ipc_can_send
	.extern sys_ipc_recv
	.extern sys_cgetc

.macro syscalltable
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
.endm


EXPORT(sys_call_table)
syscalltable
.size sys_call_table, . - sys_call_table 






