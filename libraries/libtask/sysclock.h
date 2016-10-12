/* ---------------------------------------------------------------------------- */
/* sysclock.h	definitions for sysclock.c

 12 Jun 15 dpa	Created.

*/
/* ---------------------------------------------------------------------------- */

//#pragma once  // RG added 
#ifndef SYSCLOCK_H
#define SYSCLOCK_H

extern volatile TSIZE sysclock; 
extern volatile TSIZE sampleclock;

/* ---------------------------------------------------------------------------- */

void extern (*system_interrupt)();
void extern (*user_interrupt)();

/* ---------------------------------------------------------------------------- */

int sysclock_init(void);
int sysclock_init_interrupt(void);

/* ---------------------------------------------------------------------- */

#if (MACHINE == MACH_M68K) 
#define SAMPLES_PER_TICK 8
#endif

#if (MACHINE == MACH_AVR) 
#define SAMPLES_PER_TICK 10
#endif

#if (MACHINE == MACH_ARM) 
#define SAMPLES_PER_TICK 10
#endif

/* ---------------------------------------------------------------------------- */
/* EOF sysclock.h */

#endif