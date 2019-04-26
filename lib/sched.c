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

void sched_yield(void)
{
	/*
	static u_int cur_lasttime = 1;
	struct Env *next_env;
	cur_lasttime--;
	if (cur_lasttime == 0 || curenv == NULL)
	{
		if (curenv != NULL)
		{
			LIST_INSERT_TAIL(&env_sched_list[1], curenv, env_sched_link);
		}
		if (!LIST_EMPTY(&env_sched_list[0]))
		{
			next_env = LIST_FIRST(&env_sched_list[0]);
			LIST_REMOVE(next_env, env_sched_link);
		}
		else
		{
			next_env = LIST_FIRST(&env_sched_list[1]);
			LIST_REMOVE(next_env, env_sched_link);
		}
		cur_lasttime = next_env->env_pri;
		//printf("%x %x\n",curenv, next_env);
		env_run(next_env);
	}
	env_run(curenv);
	//printf("%x",*((int*)(TF_EPC + TIMESTACK - TF_SIZE)));
	//printf("Sched_yield End\n");
	*/
	//printf("Sched_yield happen\n");
	static int cur_lasttime = 1;
	static int cur_head_index = 0;
	struct Env *next_env;
	int now_have = 0;
	cur_lasttime--;
	if (cur_lasttime <= 0 || curenv == NULL || curenv->env_status != ENV_RUNNABLE)
	{
		now_have = 0;
		while(1)
		{
			if (LIST_EMPTY(&env_sched_list[cur_head_index]))
			{
				cur_head_index = !cur_head_index;
				break;
			}
			next_env = LIST_FIRST(&env_sched_list[cur_head_index]);
			if (next_env->env_status == ENV_RUNNABLE)
			{
				now_have = 1;
				break;
			}
			LIST_REMOVE(next_env, env_sched_link);
			LIST_INSERT_HEAD(&env_sched_list[!cur_head_index], next_env, env_sched_link);
		}
		if (!now_have)
		{
			while (1)
			{
				if (LIST_EMPTY(&env_sched_list[cur_head_index]))
				{
					panic("^^^^^^No env is RUNNABLE!^^^^^^");
				}
				next_env = LIST_FIRST(&env_sched_list[cur_head_index]);
				if (next_env->env_status == ENV_RUNNABLE)
				{
					now_have = 1;
					break;
				}
				LIST_REMOVE(next_env, env_sched_link);
				LIST_INSERT_HEAD(&env_sched_list[!cur_head_index], next_env, env_sched_link);
			}
		}
		LIST_REMOVE(next_env, env_sched_link);
		LIST_INSERT_HEAD(&env_sched_list[!cur_head_index], next_env, env_sched_link);
		cur_lasttime = next_env->env_pri;
		env_run(next_env);
		panic("^^^^^^sched yield jump faild^^^^^^");
	}
	env_run(curenv);
	panic("^^^^^^sched yield reached end^^^^^^");
}
