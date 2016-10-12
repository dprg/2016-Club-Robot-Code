/* ---------------------------------------------------------------------- */
/* sysclock.c		1 kHz System clock.

22 Jul 15 dpa	Created from nbot's sysclock.c.
28 Aug 15 dpa	Mods for AVR, ARM.
11 Sep 15 dpa	AVR 10 kHz sampleclock.
*/
/* ---------------------------------------------------------------------- */

#include "task.h"
#include "log.h"
#include "sysclock.h"

/* ---------------------------------------------------------------------- */
/* #define SAMPLES_PER_TICK 10   ---	 defined in sysclock.h */
/* ---------------------------------------------------------------------- */
/* System wide globals */

volatile TSIZE sysclock;	/* 1 KHz system clock */
volatile TSIZE sampleclock;	/* 10 KHz logging clock */

void (*system_interrupt)();     /* clock system vector for 1khz interrupt */
void (*user_interrupt)();       /* clock user vector for 1khz interrupt */




/*----------------------------------------------------------------------- */
/* System Clock.  Maintains 1kHz "sysclock" timer.  			  */

int samples_per_tick;		/* 8 for M68332, 10 for AVR, 1 for ARM  */

void sysclock_driver(void)
{
	sampleclock++;

	if (--samples_per_tick < 1) {
		sysclock++;
        	if (system_interrupt) (*system_interrupt)();
        	if (user_interrupt) (*user_interrupt)();
		samples_per_tick = SAMPLES_PER_TICK;
	}
}

/* ---------------------------------------------------------------------- */
/* initialize the sysclock */

int sysclock_init(void)
{
	extern int sysclock_init_interrupt(void);
	
	sysclock = 0;  sampleclock = 0;
	system_interrupt = 0;  user_interrupt = 0;
	system_log = 0;  error_log = 0;
	samples_per_tick = SAMPLES_PER_TICK;

	return (sysclock_init_interrupt());
}

/* ---------------------------------------------------------------------- */
/*		Hardware-Specific Interrupt Initialization.		  */
/* ---------------------------------------------------------------------- */

/* Sysclock_init() exits with a call to sysclock_init_interrupt().  That is
 * a machine-specific function supplied by the user which initializes the
 * particular hardware interrupt which calls the sysclock_driver().
 *
 * The following are examples for the MC68332 processor running at 25 MHz
 * with an 8 KHz sample clock, and AVR Mega2560 processor at 16 MHz with a
 * 10 KHZ sample clock.
*/

/* ---------------------------------------------------------------------- */
/* Motorola/Coldfire MC68332 @ 25 MHz */
/* ---------------------------------------------------------------------- */

#if (MACHINE == MACH_M68K)

#include "68332/mrm332/mrm332.h" /* Hardware specific stuff for MC68332 */

__attribute__((__interrupt__)) void sysclock_handler(void) 
{
	sysclock_driver();
}

int sysclock_init_interrupt(void) 
{
	#define SIMVEC 80 	/* The MC68332 SIM module timer, interrupt vector 80. */
	int oldvec;

	/* setup the interrupt vector */
	interrupt_catch( (void*)sysclock_handler, SIMVEC, &oldvec); 

	/* initialize the interrupt timer hardware. */
	#if (SAMPLES_PER_TICK == 1)			/* 1kHz interrupt */
  	*(short int*)0xfffa24 = 8;			/* PITR, 8 = 1024 hz */
	#endif
	#if (SAMPLES_PER_TICK == 8)			/* 8kHz interrupt */
  	*(short int*)0xfffa24 = 1;			/* PITR, 1 = 8192 hz */
	#endif

	/* initialize the vector and priority */
  	*(short int*)0xfffa22 = 0x600+SIMVEC;		/* PICR control register */
							/* 0x600 is priority 6, SIMVEC is vector number */
							/* don't enable interrupts yet. */
	return 0;
}

#endif

/* ---------------------------------------------------------------------- */
/* AVR    Arduino Mega2560 @ 16 MHz */
/* ---------------------------------------------------------------------- */

#if (MACHINE == MACH_AVR)

#include "avr/machine.h"

/* Set up AVR 10kHz clock driver interrupt using TIMER2 */
/*
 * TIMER2 Description:    The pre-scaler and the timer count divide the timer-counter
 *                 clock frequency to give a timer overflow interrupt rate:
 *
 *                Interrupt rate =  16MHz / (prescaler * (255 - TCNT2))
 *
 *                TCCR2B[b2:0]   Prescaler    Freq [KHz], Period [usec] after prescale
 *                0x0            (TC stopped)     0         0
 *                0x1                1        16000.        0.0625
 *                0x2                8         2000.        0.500
 *                0x3               32          500.        2.000
 *                0x4               64          250.        4.000
 *                0x5              128          125.        8.000
 *                0x6              256           62.5      16.000
 *                0x7             1024           15.625    64.000
 */

/* Note: COUNT is 205 for 10kHz, (16000000/(32 * (255 - COUNT))). */

#define INIT_TIMER_COUNT 205
#define RESET_TIMER2 TCNT2 = INIT_TIMER_COUNT

ISR(TIMER2_OVF_vect) {
        RESET_TIMER2;
        sysclock_driver();
}

int sysclock_init_interrupt(void)
{
	TCCR2A |= (1<<CS22); 
	TCCR2A &= ~((1<<CS21) | (1<<CS20));  
	TCCR2A &= ~((1<<WGM21) | (1<<WGM20)); 
        TCCR2B = 0x03;		// divide by 32 
	TIMSK2 |= (1<<TOIE2);   //Timer2 Overflow Interrupt Enable  
	RESET_TIMER2;               
	sei();
 	return 0;
}

#endif

/* ---------------------------------------------------------------------- */
/* ARM */
/* ---------------------------------------------------------------------- */
  
#if (MACHINE == MACH_ARM)

/* TEENSY 3.1 clock driver */
#include <TimerThree.h>

int sysclock_init_interrupt(void)
{
        Timer3.initialize(100);                     // start timer 3  100us period
        Timer3.attachInterrupt(sysclock_driver);    // call sysclock_driver once every period
        
	return 0;
}


#endif

/* ---------------------------------------------------------------------- */
/* EOF sysclock.c */


