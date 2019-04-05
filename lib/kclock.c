/* See COPYRIGHT for copyright information. */

/* The Run Time Clock and other NVRAM access functions that go with it. */
/* The run time clock is hard-wired to IRQ8. */

#include <kclock.h>


extern void set_timer();

void
kclock_init(void)
{
	/* initialize 8253 clock to interrupt 100 times/sec */
	//outb(TIMER_MODE, TIMER_SEL0|TIMER_RATEGEN|TIMER_16BIT);
	//outb(IO_TIMER1, TIMER_DIV(100) % 256);
	//outb(IO_TIMER1, TIMER_DIV(100) / 256);
	//printf("	Setup timer interrupts via 8259A\n");
	set_timer();
	//irq_setmask_8259A (irq_mask_8259A & ~(1<<0));
	//printf("	unmasked timer interrupt\n");
	
}

