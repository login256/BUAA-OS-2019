/* See COPYRIGHT for copyright information. */

#ifndef __SCHED_H__
#define __SCHED_H__

void sched_init(void);
void sched_yield(void);
void sched_intr(int); 

void sched_yield_new(void);

#endif /* __SCHED_H__ */
