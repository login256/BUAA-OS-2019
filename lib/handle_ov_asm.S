#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>
#include <stackframe.h>
#include <trap.h>

NESTED(handle_ov, TF_SIZE, sp)
	.set noat
	nop
	SAVE_ALL
	CLI
	jal ov_do
	nop
	jal ov_check
	nop
	ret_from_exception
END(handle_ov)
