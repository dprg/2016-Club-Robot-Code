/* ------------------------------------------------------------------- */
/* task.h       struct definitions for LMX multi-tasking.

05 Feb 98 dpa   Created.
08 Oct 04 dpa   MRM 68332.  changed unsigned chars to ints.
20 Feb 05 dpa   Added external reference for sysclock.
20 Sep 13 dpa   nBot-m68k.  Added name and stacksize elements to TASK
12 Jun 15 dpa   added function prototypes.
22 Jul 15 dpa   Stripped out nbot specific stuff.
24 Aug 15 dpa   WAKE long.
26 Aug 15 dpa   Comments.
14 Sep 15 dpa   ARM.
*/
/* ---------------------------------------------------------------------- */
/* Define Machine Architecture */

//#pragma once  // RG added 
#ifndef TASK_H
#define TASK_H

#define MACH_M68K 0
#define MACH_ARM  1
#define MACH_AVR  2

// #define MACHINE MACH_M68K
// #define MACHINE MACH_ARM
#define MACHINE MACH_AVR

/* ------------------------------------------------------------------- */
/* Define size of timers and task arguments */

#if (MACHINE == MACH_AVR)
        #define TSIZE unsigned long
        #define ASIZE long
#else
        #define TSIZE unsigned int
        #define ASIZE int
#endif

/* ------------------------------------------------------------------- */
/* define max permissible tasks and minimum allowed stack space  */

#if (MACHINE == MACH_AVR)
        #define MAXTASKS 12
        #define MINSTACK 64
#else
        #define MAXTASKS 32
        #define MINSTACK 128
#endif


/* ------------------------------------------------------------------- */
/* The TASK data struct. */

typedef struct task TASK;

struct task
{
        TASK    *next;          /* pointer to next TASK struct in linked list */
        void    (*func)(ASIZE arg);     /* pointer to function */
        ASIZE   arg;            /* initial argument to function */
        int     pid;            /* This task's Process ID */
        int     state;          /* Task state, 0=NEWTASK,1=RUNNING,2=WAITING,4=BLOCKING */
        int     *fp;            /* storage for processor frame pointer register. */
        int     stacksize;      /* requested stack size */
        int     *block;         /* block pointer */
        TSIZE   wake;           /* 32bit wakeup time */
        char    *name;          /* pointer to ASCII string lable for unique task name */
        int    stack[1];        /* stack+stacksize starts here. */
};

 
/* ------------------------------------------------------------------- */
/* Offsets for assembly instructions */

#if (MACHINE == MACH_AVR)

/* 16bit: AVR */
asm("NEXT=0");asm("FUNC=2");asm("ARG=4");asm("PID=8");asm("STATE=10");
asm("FP=12");asm("SSIZE=14");asm("BLOCK=16");asm("WAKE=20");asm("NAME=22");asm("STACK=24");

#else

/* 32bit: M68K, ARM */
asm("NEXT=0");asm("FUNC=4");asm("ARG=8");asm("PID=12");asm("STATE=16");
asm("FP=20");asm("SSIZE=24");asm("BLOCK=28");asm("WAKE=32"); asm("NAME=36");asm("STACK=40");

#endif

/* ------------------------------------------------------------------- */
/* define task states */

#define NEWTASK 0               /* Task is uninitialized. */
#define RUNNING 1               /* Task is running */
#define WAITING 2               /* Task is waiting on a timer tick */
#define BLOCKING 4              /* Task is blocking */

/* and in assembly */
asm("NEWTASK=0");asm("RUNNING=1");asm("WAITING=2");asm("BLOCKING=4");

/* ------------------------------------------------------------------- */
/* task.c globals */

extern TASK *current;           /* pointer to currently executing task */
extern int pid_count;           /* Process ID counter */

extern TSIZE proc_counter;      /* null_task()'s idle rate counter */
extern char run_level;          /* 0 = single task, 1 = multi-task */

/* ------------------------------------------------------------------- */
/*              Create, start, and kill process tasks                  */
/* ------------------------------------------------------------------- */


int create_task(char *name,void (*func)(ASIZE arg), ASIZE arg, int stack_size);

/*  Initialize a task struct and add it to the linked list.  The task must
    exit with terminate() or else loop endlessly around defer() calls. 
    Expects:
        char    *name   Pointer to string, unique ASCII lable for this task.
        void (*func)(ASIZE arg) Pointer to task function.
        ASIZE arg       Initial argument to pass to task function at startup.
        int stack_size  Requested stack space.  Minimum is app-dependent.
   Returns: process ID of the new task if successful, else -1 calloc error. */

#define NULL_TASK 0
#define NO_TASK 1

void scheduler(void);
/* Start multi-tasking.  Call this last in main.  Does not exit */

int kill_process(int pid);
/*  Kill a process by its process ID.  Expects a PID returned by create_task().  
    Frees the TASK struct of 'pid', returns 0 on success, else -1 */

void terminate();
/* Kill self. */

/* ------------------------------------------------------------------- */
/*                      Suspend Self                                   */
/* ------------------------------------------------------------------- */

void wake_after(TSIZE delay);
/* Suspend self for 'delay' milliseconds */

int  semaphore_obtain(int *s);
/* Suspend self blocking on a semaphore pointed by 's' */

int semaphore_obtain_timeout(int *s, TSIZE timeout);
/* Suspend self blocking on a semaphore pointed by 's' with a timeout of 'timeout' ms.
   returns 0 = block cleared, 1 = timeout expired. */

void semaphore_release(int *s);
/* Release a semaphore pointed by 's' */

void msleep(TSIZE delay);
/* Call defer() in a tight loop while waiting on a timer tick */

void defer(void);

/* Execute next task in round-robin queue. */ 

/* ------------------------------------------------------------------- */
/* user utilities */
/* ------------------------------------------------------------------- */

TASK *findpid(int pid);
/* return the TASK pointer for a given task ID, else return -1 */

TASK *findprev(TASK *t);
/* return the TASK pointer to the previous task in the linked list */

int stack_usage(int pid);
/* Return number of stack bytes used by a given task ID. */

int iterate_tasks(void (*func)(TASK *t, int arg), int arg);
/* Iterate across tasks list calling (*func)(TASK *t, int arg) for each task. */

/* ------------------------------------------------------------------- */
/* system utilities */
/* ------------------------------------------------------------------- */

void do_timers(void);
/* Test and unblock tasks waiting on timer tick if ready */

void null_task(ASIZE ignored);
/* Default system task started by scheduler(), always runs, counts idle cycles. */

/* ------------------------------------------------------------------- */
/* EOF task.h */
#endif
