/* ---------------------------------------------------------------------- */
/* task.c       A Lightweight Multitasking Executive (LMX)                */
/*              Suitable for Robotics using Embedded Processors.          */
/*              by David P. Anderson                                      */
/* ---------------------------------------------------------------------- */
/* 
A real-time lightweight multitasking executive is implemented by the code
in this file.  It uses a linked list of data structures and stacks to
allow real-time cooperative tasks to run round-robin in parallel.

This works in conjunction with a 1kHz system 'sysclock' timer, provided
by the companion code in "sysclock.c" and "sysclock.h", to implement
millisecond timed delays and periodic execution.

The file "doc/README-LMX.tst" contains user documentation of the executive,
a brief theory of operation, and some example code.
*/
/* ---------------------------------------------------------------------- */
/* History

05 Feb 98 dpa   Ported from Forth, inspired by Circuit Cellar article by Steve Ciarcia.
08 Oct 04 dpa   Ported to M68332 from SR04 C code for the Motorola 68HC11.
20 Sep 13 dpa   nBot-m68k.  Added stack diagnostic.
30 Sep 13 dpa   system_log.
13 Oct 14 dpa   Semaphores.
16 Jul 15 dpa   MC68332 Assembly version of defer() code is x2 faster.
20 Jul 15 dpa   MC68332 assembly version of do_timers() is marginally faster.
                Added conditional compile flag USE_M68K_ASM for assembly code.
23 Jul 15 dpa   Cleaned up comments for public consumption.
08 Aug 15 dpa   Fixed memory leak in terminate(); renamed do_block();
24 Aug 15 dpa   Mods for AVR.
29 Aug 15 dpa   ENABLE_TIMERS, DISABLE_TIMERS
03 Sep 15 dpa   null_pid, null_tptr.
07 Sep 15 dpa   New defer();
14 Sep 15 dpa   ARM.
20 Sep 15 dpa	Don't delete last remaining task.
*/
/* ---------------------------------------------------------------------- */

#define VERSION "3.440"

/* ---------------------------------------------------------------------- */

#include <stdlib.h>
#include "task.h"
#include "log.h"
#include "sysclock.h"

/* ---------------------------------------------------------------------- */
/* Load processor-dependent macros */

#if (MACHINE == MACH_M68K)
        #include "68332/machine.h"
#endif

#if (MACHINE == MACH_ARM)
        #include "arm/machine.h"
#endif

#if (MACHINE == MACH_AVR)
        #include "avr/machine.h"
        char sreg;
#endif

/* ---------------------------------------------------------------------- */
/* stack usage diagnostic */

#define STACKFREE 0xDEAD
short stack_diag = 1;

/* ---------------------------------------------------------------------- */
/* globals */

TASK *current;          /* pointer to currently executing task */
int pid_count;          /* process id counter */

char run_level;         /* 0 = single task,  1 = multi-task */
char run_level_state;

int null_pid;           /* process ID of null task */
TASK *null_tptr;        /* task pointer of null task */
TSIZE proc_counter;     /* idle task counter */

/*----------------------------------------------------------------------- */
/* defer()      do a process context switch. */
/*----------------------------------------------------------------------- */
/*
        Called from cooperative multi-tasks (round robin).

        This code uses machine-specific assembly instruction macros
        from "machine.h" to load and store the processor's stack frame 
	and pointer registers.  See the file "doc/README-machine.txt"
	for details.
*/
/* ---------------------------------------------------------------------- */
/* defer to next running task in linked list */

void defer(void)
{
        TASK *t;

        if (run_level) {
                t = current->next;
                while (t->state != RUNNING) {
                        if  (t->state == NEWTASK)  { 
                                STORE_FRAME;
                                current = t;
                                NEW_TASK; 
                                break; 
                        }
                        if ((t->state & BLOCKING) 
                           && (*(t->block) == 0)) { 
                                t->state = RUNNING;
                                break; 
                        }
                        t = t->next;
                }
                if (t != current ) {
                        STORE_FRAME;
                        current = t;
                        LOAD_FRAME; 
                }
        }
}

/*----------------------------------------------------------------------- */
/* do_timers();  Run this from 1kHz sysclock system_interrupt vector      */
/*----------------------------------------------------------------------- */

void do_timers(void)
{
        TASK *p;

        if (run_level) {
            p = null_tptr;
            while ((p = p->next) != null_tptr) {
                if ((p->state & WAITING) && (sysclock >= p->wake)) {
                        p->state = RUNNING;
                }
            }
        }
}

/* -------------------------------------------------------------------- */
/*                              Scheduler                               */
/* run this last in main, starts multi-tasking, should not exit         */
/*                                                                      */
/* -------------------------------------------------------------------- */

void scheduler(void)
{
        DISABLE_TIMERS;

        null_pid = create_task("Null",null_task,0,MINSTACK); 
        if (null_pid <= 0) return;

        null_tptr = findpid(null_pid);
        if (null_tptr == (TASK*)-1) return;

        system_interrupt = do_timers;
        run_level_state = 1;
        ENABLE_TIMERS;

        NEW_TASK;
}

/* --------------------------- */

void null_task(ASIZE ignored)
{
        proc_counter = 0;
        while(1) {
                proc_counter++;
                defer();
        }
}

/*----------------------------------------------------------------------- */
/*               suspend self in the round-robin queue                    */
/*----------------------------------------------------------------------- */
/*
These routines call the macro 'YIELD' which is defined in "log.h" as:

#define YIELD { LOGON; defer(); LOGOFF; }

It can be used in conjuction with the realtime logging vectors defined in
"log.c" to measure system timing, calculate per-task and total cpu usage,
and track system timing errors.  

The "log.h" file also includes the macro 'PERIOD(*a,b)' which provides 
periodic execution of the calling task.
*/
/* ---------------------------------------------------------------------- */
/* suspend for 'delay' milliseconds */

void wake_after(TSIZE delay)
{
        current->wake = sysclock + delay;
        current->state = WAITING;
        YIELD;
}

/* ----------------------------------------- */
/* suspend until semaphore 's' is cleared    */
/* always returns 0                          */

int semaphore_obtain(int *s)
{
        *s = 1;
        current->block = s;
        current->state = BLOCKING;
        YIELD;
        return 0;
}

/* ----------------------------------------- */
/* clear semaphore 's'                       */

void semaphore_release(int *s)
{
        YIELD;
        *s = 0;
}

/* ----------------------------------------- */
/* suspend until semaphore 's' is cleared    */
/*    or 'timeout' milliseconds has expired. */
/* return 0 = block cleared, 1 = expired     */

int semaphore_obtain_timeout(int *s, TSIZE timeout)
{
        *s = 1;
        current->block = s;
        current->wake = sysclock + timeout;
        current->state = BLOCKING | WAITING;
        YIELD;
        return *s;
}

/* ----------------------------------------- */
/* defer() for 'delay' milliseconds          */

void msleep(TSIZE delay)
{
        TSIZE s = sysclock + delay;
        while (s > sysclock) { YIELD; }
}

/*----------------------------------------------------------------------- */
/* create and initialize a TASK structure.                                */
/*                                                                        */
/* These functions use libc calloc() and free() memory management calls.  */
/* This requires you have a heap set up for your execution environment    */
/* and access to <stdlib.h> and libc/newlib or similar.                   */
/*                                                                        */
/*----------------------------------------------------------------------- */

/* Allocate TASK struct and 'stack-size' stack, and fill memory */
/* return pointer to TASK struct, else -1 on calloc error. */

static TASK *alloc_task_struct(int stack_size)
{
        TASK *p;
        int i,j;
        short *ss;
        
        p = (TASK *)(void*)calloc(1, sizeof(TASK) + (stack_size*sizeof(int)));
        if (p == (TASK*)NULL) return (TASK*)-1;

        if (stack_diag) {
                ss = (short *)p;
                j = (sizeof(TASK)/sizeof(short)) 
                        + (stack_size*sizeof(int))/sizeof(short);
                for (i = 0; i < j; i++) ss[i] = STACKFREE;
        }

        return p;
}

/* Load task parameters to struct 'p', clear 'state' and 'block' */
/* and set 'pid' from global pid counter */

static void initialize_task_struct
(TASK *p,char *name, void (*func)(ASIZE arg), ASIZE arg, int stacksize) {
        p->name = name;
        p->func = func;
        p->arg = arg;
        p->state = 0;
        p->block = 0;
        p->wake = 0;
        p->stacksize = stacksize;
	/* find unused pid & handle overflow */
        if (pid_count) {
                do { if ((++pid_count) < 0) pid_count = 1;
                } while ((findpid(pid_count)) != (TASK*)-1);
        } else  { ++pid_count; }

        p->pid = pid_count;
}

/* Initialize task stack frame 'p' and FP and SP registers */

static void initialize_task_frame(TASK *p)
{
        int *s;
        s = &p->stack[p->stacksize-1];  /* get task stack address to s */

        #if (MACHINE == MACH_M68K)
                /* empty descending stack */
                *s-- = 0;
                *s-- = (int) &p->stack[p->stacksize-2];
                *s-- = (int) &p->stack[p->stacksize-3];
        #endif
        #if (MACHINE == MACH_ARM)
                /* empty descending stack */
                *s-- = p->arg;
                *s-- = (int) &p->stack[p->stacksize-1];
        #endif
        #if (MACHINE == MACH_AVR)
                /* full descending stack */
                *(  s) = 0;
                *(--s) = (int) &p->stack[p->stacksize-2];
                *(--s) = (int) &p->stack[p->stacksize-3];
        #endif
        p->fp = s;                              /* use s as initial fp */
}


/*----------------------------------------------------------------------- */
/* Insert and delete task link in linked list.                            */
/*----------------------------------------------------------------------- */

/* Add  a task to the linked list:     */
void insert_task_link(TASK *p)
{
        DISABLE_TIMERS;
        if (current == 0) {	/* initialize current to first task created */
                current = p;	
                p->next = p;	/* point tail to head */
        } else {
                p->next = current->next; /* insert between current and next */
                current->next = p;
        }
        ENABLE_TIMERS;
}

/* Remove a task from the linked list: */
void delete_task_link(TASK *p)
{
        TASK *t;
        t = findprev(p);	/* t = previous task in linked list */

        DISABLE_TIMERS;
	if (t == p) return; 	/* don't kill last task. */
        t->next = p->next;	/* link around p */

        if (p == current) {	/* did we link around calling task? */
                current = t->next;
                ENABLE_TIMERS;
                free(p);
                NEXT_FRAME;	/* execute next frame */
        } else  {
                ENABLE_TIMERS;
                free(p);
        }
}

/*----------------------------------------------------------------------- */
/* Create a new task. */
/*----------------------------------------------------------------------- */

int create_task(char *name, void (*func)(ASIZE arg), ASIZE arg, int stacksize)
{
/* 
Expects:        char *name              pointer to unique ascii lable for this task. 
                void (*func)(ASIZE arg) pointer to the task function.
                ASIZE arg               initial argument to pass to the task function.
                int stacksize           requested stack size for this task.

Returns:        process ID if successful, else -1 for calloc error.
*/
        TASK *p;

        if ((p = alloc_task_struct(stacksize)) == (TASK*)-1) return -1;
        initialize_task_struct(p,name,func,arg,stacksize);
        initialize_task_frame(p);
        insert_task_link(p);

        return p->pid;
}

/*----------------------------------------------------------------------- */
/* Kill a task by it's process id, as returned by create_task();          */
/*----------------------------------------------------------------------- */

int kill_process(int pid) /* kill task by pid.  Return 0 on success, else -1  */
{
        TASK *t;
        if ((pid < 1) || ((t = findpid(pid)) == (TASK*)-1)) return -1;
        delete_task_link(t);
        return 0;
}


void terminate()        /* kill self */
{
        kill_process(current->pid);
}

/* ---------------------------------------------------------------------- */
/*                      TASK/Process ID Tools                             */
/* ---------------------------------------------------------------------- */

TASK *findpid(int pid)  /* return task pointer for process ID 'pid'  */ 
{                       /* or -1 if not found.  */
        TASK *p;
        p = current;
        while (p->pid != pid) {
                p = p->next;
                if (p == current) {
                        p = (TASK*)-1;
                        break;
                }
        }
        return p;
}

TASK *findprev(TASK *t) /* return pointer to previous task in linked list */
{
        TASK *p;
        p = t;
        while (p->next != t) p = p->next;
        return p;
}

/* ---------------------------------------------------------------------- */
/* calculate and return stack usage for a given process ID */
/* ---------------------------------------------------------------------- */

int stack_usage(int pid)
{
        TASK *t;
        unsigned short *s;
        int i,j;

        if (stack_diag) {
                t = findpid(pid);
                if (t != (TASK*)-1) {
                        s = (unsigned short *)&t->stack[0];

                        j = (t->stacksize*sizeof(int))/sizeof(short);
                        for (i = 0; i < j; i++) {
                                if (s[i] != STACKFREE) break;
                        }
                        return (t->stacksize - i/((sizeof(int)/sizeof(short))));
                } 
        } 
        return -1;
}


/*----------------------------------------------------------------------- */
/* Iterate across tasks.  Expects function of the form:
        function(TASK *t, int argument);
        Executes function once for each task in the linked list.
*/

int iterate_tasks(void (*func)(TASK *t, int arg), int arg)
{
        TASK *t;
        t = current;
        do {
                (*func)(t,arg);
                t = t->next;
        } while (t != current);
        return 0;
}

/*----------------------------------------------------------------------- */
/* eof */
