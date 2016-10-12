/* ----------------------------------------------------------------- */
/* log.h	definitions

11 Jul 2015 dpa	Created.
01 Sep 2015 dpa print vector.  print_llist(), print_task().

*/
/* ----------------------------------------------------------------- */

//#pragma once
#ifndef LOG_H
#define LOG_H

/* ----------------------------------------------------------------- */

#include <stdio.h>

/* ----------------------------------------------------------------- */

extern void (*system_log)(int flag);
extern void (*error_log)(int pid, int err);
extern void (*printv)(char *s);
extern char sbuf[];

/* ----------------------------------------------------------------- */
/* kernel print sbuf[] vector */

#define PRINTS(s) (*printv)(s)

/* ----------------------------------------------------------------- */
/* Log defs  */

#if (MACHINE == MACH_AVR)
	#define LOG_ON  0x8000
	#define LOG_OFF 0x0000
#else
	#define LOG_ON  0x80000000
	#define LOG_OFF 0x00000000
#endif

#define LOGON  {if (system_log) (*system_log)(LOG_ON); }
#define LOGOFF {if (system_log) (*system_log)(LOG_OFF); }
#define LOGERR(a,b) {if (error_log) (*error_log)(a,b); }

/* ----------------------------------------------------------------- */

#define YIELD    { LOGON; defer(); LOGOFF; }

#define DELAY(d) { wake_after(d); }

#define PERIOD(a,b) { if (*a < sysclock ) \
		{ LOGERR(current->pid,*a);  YIELD; } \
		else { DELAY(*a - sysclock); } \
		*a += b; }

/* ----------------------------------------------------------------- */

#define NO_VERBOSE 0
#define VERBOSE 1

void print_llist(int flag);

/* ----------------------------------------------------------------- */
/* EOF */

#endif