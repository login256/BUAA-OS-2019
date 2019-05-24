/* See COPYRIGHT for copyright information. */

#ifndef _KCLOCK_H_
#define _KCLOCK_H_
#define	IO_RTC		0xb5000100		/* RTC port */
#ifndef __ASSEMBLER__
void kclock_init(void);
#endif /* !__ASSEMBLER__ */
#endif
