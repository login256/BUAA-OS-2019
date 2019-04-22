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
	jal ov_check
	nop
	j ret_from_exception
	nop
END(handle_ov)
