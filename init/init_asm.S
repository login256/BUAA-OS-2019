#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>

LEAF(add_ov)
	subu sp, sp, 40
	sw t0, 0(sp)
	sw t1, 4(sp)
	sw t2, 8(sp)
	sw t3, 12(sp)
	sw t4, 16(sp)
	sw t5, 20(sp)
	sw t6, 24(sp)
	sw t7, 28(sp)
	sw ra, 32(sp)

	li t1, 0x7ffffffe
	li t2, 0x7ffffffe
	add t0, t1, t2

	lw t0, 0(sp)
	lw t1, 4(sp)
	lw t2, 8(sp)
	lw t3, 12(sp)
	lw t4, 16(sp)
	lw t5, 20(sp)
	lw t6, 24(sp)
	lw t7, 28(sp)
	lw ra, 32(sp)
	addu sp, sp, 40
	jr ra
	nop
END(add_ov)

