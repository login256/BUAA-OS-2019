/* Notes written by Qian Liu <qianlxc@126.com>
  If you find any bug,please contact this email.*/

#include <mmu.h>
#include <error.h>
#include <env.h>
#include <pmap.h>
#include <printf.h>

struct Env *envs = NULL;		// All environments
struct Env *curenv = NULL;	        // the current env


