/* ---------------------------------------------------------------------------- */
/* machine.h            LMX "task.c" Macros for AVR processors                 */
/* ---------------------------------------------------------------------------- */
/*
23 Aug 2015 dpa         Created.
27 Aug 2015 dpa         LOAD_FRAME, STORE_FRAME, UNLINK_TASK.
29 Aug 2015 msims,dpa	Reformated, ENABLE_TIMERS, DISABLE_TIMERS.
			(*current->func)(current->arg);
05 Sep 2015 dpa		STORE_SP, LOAD_SP
07 Sep 2015 dpa		New defer() macros.
*/
/* ---------------------------------------------------------------------------- */

#define __AVR_ATmega2560__
#include  <avr/interrupt.h>
#include  <avr/io.h>

/* ---------------------------------------------------------------------------- */

#define SAVE_REGS       asm volatile ("mov r4, r28"); \
			asm volatile ("mov r5, r29"); \
			asm volatile ("mov r6, r30"); \
			asm volatile ("mov r7, r31")

#define LOAD_REGS	asm volatile ("mov r31, r7"); \
			asm volatile ("mov r30, r6"); \
			asm volatile ("mov r29, r5"); \
			asm volatile ("mov r28, r4")

#define STORE_SP	{ SAVE_REGS; \
			asm volatile ("in r28, __SP_L__"); \
                        asm volatile ("in r29, __SP_H__"); \
			asm volatile ("lds r30,current");  \
                        asm volatile ("lds r31,current+1");\
                        asm volatile ("std Z+FP,   r28");  \
                        asm volatile ("std Z+FP+1, r29");  \
			LOAD_REGS; }

#define LOAD_SP		{ SAVE_REGS; \
			asm volatile ("lds r30,current"); \
                        asm volatile ("lds r31,current+1"); \
                        asm volatile ("ldd r28, Z+FP"); \
                        asm volatile ("ldd r29, Z+FP+1"); \
                        asm volatile ("out __SP_L__, r28"); \
                        asm volatile ("out __SP_H__, r29"); \
			LOAD_REGS; }

#define UNLINK		asm volatile ("pop r29"); \
                        asm volatile ("pop r28")

#define FIX_FRAME	asm("nop")

#define RETURN          asm volatile ("ret")

#define PUSH_REGS	asm volatile ("push r0"); \
			asm volatile ("push r1"); \
			asm volatile ("push r2"); \
			asm volatile ("push r3"); \
			asm volatile ("push r4"); \
			asm volatile ("push r5"); \
			asm volatile ("push r6"); \
			asm volatile ("push r7"); \
			asm volatile ("push r8"); \
			asm volatile ("push r9"); \
			asm volatile ("push r10"); \
			asm volatile ("push r11"); \
			asm volatile ("push r12"); \
			asm volatile ("push r13"); \
			asm volatile ("push r14"); \
			asm volatile ("push r15"); \
			asm volatile ("push r16"); \
			asm volatile ("push r17"); \
			asm volatile ("push r18"); \
                        asm volatile ("push r19"); \
                        asm volatile ("push r20"); \
                        asm volatile ("push r21"); \
                        asm volatile ("push r22"); \
                        asm volatile ("push r23"); \
                        asm volatile ("push r24"); \
                        asm volatile ("push r25"); \
                        asm volatile ("push r26"); \
                        asm volatile ("push r27"); \
                        asm volatile ("push r28"); \
                        asm volatile ("push r29"); \
                        asm volatile ("push r30"); \
                        asm volatile ("push r31")
                                     
#define POP_REGS        asm volatile ("pop r31"); \
                        asm volatile ("pop r30"); \
                        asm volatile ("pop r29"); \
                        asm volatile ("pop r28"); \
                        asm volatile ("pop r27"); \
                        asm volatile ("pop r26"); \
                        asm volatile ("pop r25"); \
                        asm volatile ("pop r24"); \
                        asm volatile ("pop r23"); \
                        asm volatile ("pop r22"); \
                        asm volatile ("pop r21"); \
                        asm volatile ("pop r20"); \
                        asm volatile ("pop r19"); \
                        asm volatile ("pop r18"); \
                        asm volatile ("pop r17"); \
                        asm volatile ("pop r16"); \
                        asm volatile ("pop r15"); \
                        asm volatile ("pop r14"); \
                        asm volatile ("pop r13"); \
                        asm volatile ("pop r12"); \
                        asm volatile ("pop r11"); \
                        asm volatile ("pop r10"); \
                        asm volatile ("pop r9"); \
                        asm volatile ("pop r8"); \
                        asm volatile ("pop r7"); \
                        asm volatile ("pop r6"); \
                        asm volatile ("pop r5"); \
                        asm volatile ("pop r4"); \
                        asm volatile ("pop r3"); \
                        asm volatile ("pop r2"); \
                        asm volatile ("pop r1"); \
                        asm volatile ("pop r0")

/* ---------------------------------------------------------------------- */

#define DISABLE_INT	{ sreg = SREG; cli(); }
#define ENABLE_INT	{ SREG = sreg; }

#define STORE_FRAME     { DISABLE_INT; PUSH_REGS; STORE_SP; ENABLE_INT; }
#define LOAD_FRAME      { DISABLE_INT; LOAD_SP; POP_REGS; ENABLE_INT; }
                                     
#define SETSTATE        { current->state = RUNNING; }
#define EXECUTE		{ (*current->func)(current->arg); }

#define NEW_TASK        { SETSTATE; LOAD_SP; EXECUTE; }
#define NEXT_FRAME	{ LOAD_FRAME; FIX_FRAME; UNLINK; RETURN; }

#define DISABLE_TIMERS	{ run_level_state = run_level; run_level = 0; }
#define ENABLE_TIMERS	{ run_level = run_level_state; }


/* ---------------------------------------------------------------------------- */
/* EOF */
