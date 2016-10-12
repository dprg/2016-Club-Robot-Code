/* ---------------------------------------------------------------------------- */
/* log.c	provide user assignable vector for system logging		*/
/*

11 Jul 2015 dpa	Created.  Removed includes from ../src/nbot.h, now ../include/log.h
01 Sep 2015 dpa	print vector.  print_llist(), print_task().
18 Sep 2015 dpa	verbose flag for print_llist(), mods to ARM for big numbers.
*/
/* ---------------------------------------------------------------------------- */

#include <stdio.h>
#include "task.h"
#include "log.h"

/* ---------------------------------------------------------------------------- */

void (*system_log)(int flag);           /* vector for system logging */
void (*error_log)(int pid, int err);    /* vector for error logging */

/* ---------------------------------------------------------------------------- */

void (*printv)(char *s);		/* print vector for sbuf */
char sbuf[128];				/* kernel logging output buffer */

/* ---------------------------------------------------------------------------- */

void print_task(TASK *t, int ignored)
{
#if (MACHINE == MACH_M68K)
        siprintf(sbuf,"# %d\t%s\t%p\t%p\t%p\t%p\t%p\t%u\t%d\t%d\t%d\t%d",
        t->pid, t->name, t, t->next, t->func, t->fp, t->block, t->wake, 
	t->arg, t->state, t->stacksize, stack_usage(t->pid));
#endif

#if (MACHINE == MACH_AVR)
        sprintf(sbuf,"# %d\t%s\t%p\t%p\t%p\t%p\t%p\t%ld\t%ld\t%d\t%d\t%d",
        t->pid, t->name, t, t->next, t->func, t->fp, t->block, t->wake, 
	t->arg, t->state, t->stacksize, stack_usage(t->pid));
#endif

#if (MACHINE == MACH_ARM)
        // sprintf(sbuf,"# %d\t%s\t%p\t%p\t%p\t%p\t%010p\t%8ld\t%ld\t%d\t%d\t%d",
        sprintf(sbuf,"# %d\t%s\t%010p  %010p  % 10p  %010p  % 10p  % 10ld  % 10ld\t%1d\t%04d\t%04d",
        t->pid, t->name, t, t->next, t->func, t->fp, t->block, t->wake, 
	t->arg, t->state, t->stacksize, stack_usage(t->pid));
#endif

	YIELD;
        PRINTS(sbuf);
	YIELD;
}

/* ---------------------------------------------------------------------------- */

void print_task0(TASK *t, int ignored)
{
#if (MACHINE == MACH_M68K)
        siprintf(sbuf,"# %d\t%s\t%d\t%d",
        t->pid, t->name, t->stacksize, stack_usage(t->pid));
#else
        sprintf(sbuf,"# %d\t%s\t%d\t%d",
        t->pid, t->name, t->stacksize, stack_usage(t->pid));
#endif
	YIELD;
	PRINTS(sbuf);
	YIELD;
}

/* ---------------------------------------------------------------------------- */

#define NO_VERBOSE 0
#define VERBOSE 1

void print_llist(int flag)
{
#if (MACHINE == MACH_M68K)
	if (flag == VERBOSE) {
            siprintf(sbuf,"# PID\tNAME\tTASK\tNEXT\tFUNC\tFP\tBLOCK\tWAKE\tARG\tSTATE\tSTACK\tUSED");
	} else	{
            siprintf(sbuf,"# PID\tNAME\tSTACK\tUSED");
	}
#endif

#if (MACHINE == MACH_AVR)
	if (flag == VERBOSE) {
            sprintf(sbuf,"# PID\tNAME\tTASK\tNEXT\tFUNC\tFP\tBLOCK\tWAKE\tARG\tSTATE\tSTACK\tUSED");
	} else	{
            sprintf(sbuf,"# PID\tNAME\tSTACK\tUSED");
	}
#endif

#if (MACHINE == MACH_ARM)
	if (flag == VERBOSE) {
            // sprintf(sbuf,"# PID\tNAME\tTASK\t\tNEXT\t\tFUNC\tFP\t\tBLOCK\t\tWAKE\tARG\tSTATE\tSTACK\tUSED");
            sprintf(sbuf,"# PID\tNAME\t      TASK\t  NEXT\t      FUNC\t    FP\t     BLOCK\t  WAKE\t       ARG    STATE    STACK\tUSED");
	} else	{
            sprintf(sbuf,"# PID\tNAME\tSTACK\tUSED");
	}
#endif

        PRINTS(sbuf);
	if (flag == VERBOSE) {
        	iterate_tasks(print_task,0);
	} else	{
        	iterate_tasks(print_task0,0);
	}
}

/* ---------------------------------------------------------------------------- */
/* EOF */
