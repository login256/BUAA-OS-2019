#include <asm/regdef.h>
#include <asm/cp0regdef.h>
#include <asm/asm.h>


			.section .data.stk
KERNEL_STACK:
			.space 0x8000


			.text
LEAF(_start) /*LEAF is defined in asm.h and LEAF functions don't call other functions*/                       
	
	.set	mips2      /*.set is used to instruct how the assembler works and control the order of instructions */
	.set	reorder

	/* Disable interrupts */
	mtc0	zero, CP0_STATUS

        /* Disable watch exception. */
        mtc0    zero, CP0_WATCHLO
        mtc0    zero, CP0_WATCHHI

	/* disable kernel mode cache */
	mfc0	t0, CP0_CONFIG
	and	t0, ~0x7
	ori	t0, 0x2
	mtc0	t0, CP0_CONFIG
	
/*To do: 
  set up stack 
  you can reference the memory layout in the include/mmu.h
*/

			
loop:
	j	loop
	nop
END(_start)            /*the function defined in asm.h*/
