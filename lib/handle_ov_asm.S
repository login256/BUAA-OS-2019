#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>
#include <stackframe.h>
#include <trap.h>

.extern ov_do

LEAF(handle_ov)
	.set noat
	nop
	SAVE_ALL
	CLI
	move a0, sp
	jal ov_do
	nop
	li t0, 0
	li t1, 0
	li t2, 0
	li t3, 0
	li t4, 0
	li t5, 0
	li t6, 0
	li t7, 0
	jal ov_check
	nop
	j ret_from_exception
	nop
END(handle_ov)
