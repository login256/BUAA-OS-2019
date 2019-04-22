#include <env.h>
#include <pmap.h>
#include <printf.h>

/* Overview:
 *  Implement simple round-robin scheduling.
 *  Search through 'envs' for a runnable environment ,
 *  in circular fashion statrting after the previously running env,
 *  and switch to the first such environment found.
 *
 * Hints:
 *  The variable which is for counting should be defined as 'static'.
 */

extern struct Env_list env_sched_list[];
extern struct Env *curenv;

void sched_yield_new(void)
{
	static u_int cur_lasttime = 1;
	static int cur_head_index = 0;
	struct Env *next_env;
	cur_lasttime--;
	if (cur_lasttime == 0 || curenv == NULL)
	{
		if (curenv != NULL)
		{
			curenv->env_pri--;
			if(curenv->env_pri == 0)
			{
				env_destroy(curenv);
				//won't return;
				panic("sched_yield_new use env_destroy but return!");
			}
			LIST_INSERT_HEAD(&env_sched_list[!cur_head_index], curenv, env_sched_link);
		}
		if (LIST_EMPTY(&env_sched_list[cur_head_index]))
		{
			cur_head_index = !cur_head_index;
		}
		if (LIST_EMPTY(&env_sched_list[cur_head_index]))
		{
			//panic("^^^^^^No env is RUNNABLE!^^^^^^");
		}
		next_env = LIST_FIRST(&env_sched_list[cur_head_index]);
		LIST_REMOVE(next_env, env_sched_link);
		cur_lasttime = next_env->env_pri;
		env_run(next_env);
	}
	env_run(curenv);
	panic("^^^^^^sched yield reached end^^^^^^");
}
