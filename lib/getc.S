#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>

.set noreorder
LEAF(sys_cgetc)

1:	lb	t0, 0x90000000
	beqz	t0, 1b
	nop
	move	v0,t0
	jr	ra
	nop
END(sys_cgetc) 
