/* ---------------------------------------------------------------------------- */
/* machine.h    LMX "task.c" Macros for the ARM processor                      */
/* ---------------------------------------------------------------------------- */
/*
04 Aug 2015 dpa         Created from  "./arm/README.arm"
05 Aug 2015 dpa         TASK pointer dereferencing  (i.e., ldr r0,=current)
07 Aug 2015 dpa         Tested on BeagleBoneBlack.  It works!
27 Aug 2015 dpa         STORE_FRAME, LOAD_FRAME, UNLINK_TASK.
29 Aug 2015 dpa         DISABLE_TIMERS and ENABLE_TIMERS
                        (*current->func)(current->arg);
14 Sep 2015 dpa         New defer()
15 Sep 2015 dpa		Thumb mods.
*/
/* ---------------------------------------------------------------------------- */

#pragma once  // RG added 

#define DISABLE_INT     { asm("nop");}
#define ENABLE_INT      { asm("nop");}

#define STORE_FP        asm("ldr r0,=current; ldr r0, [r0, #0]; str fp, [r0, #FP]") 
#define LOAD_FP         asm("ldr r0,=current; ldr r0, [r0, #0]; ldr fp, [r0, #FP]") 

#define LINK            asm("stmfd sp!,{fp, lr}; mov fp, sp;  add fp, fp, #4")
#define UNLINK          asm("sub fp, fp, #4; mov sp, fp; add fp, fp, #4; ldmfd sp!, {fp, lr}") 

#define UNLINK_TASK     asm("add fp, fp, #4; mov sp, fp; ldmfd sp!, {fp}")
#define LOAD_TASK       asm("ldmfd sp!, {r0}; ldr lr,=current; ldr lr, [lr, #0];ldr lr, [lr, #FUNC]")
                 
#define PUSH_REGS       asm("stmfd sp!, {r4-r12}")
#define POP_REGS        asm("ldmfd sp!, {r4-r12}")

#define FIX_FRAME       asm("nop")
#define RETURN          asm("bx lr")

/* ---------------------------------------------------------------------------- */

#define STORE_FRAME     { DISABLE_INT; PUSH_REGS; LINK; STORE_FP; ENABLE_INT; }
#define LOAD_FRAME      { DISABLE_INT; LOAD_FP; UNLINK; POP_REGS; ENABLE_INT; }

#define SETSTATE        { current->state = RUNNING; }
#define NEW_TASK        { SETSTATE; LOAD_FP; UNLINK_TASK; LOAD_TASK; RETURN; }
#define NEXT_FRAME      { LOAD_FRAME; FIX_FRAME; RETURN; }

#define DISABLE_TIMERS  { run_level_state = run_level; run_level = 0; }
#define ENABLE_TIMERS   { run_level = run_level_state; }

/* ---------------------------------------------------------------------------- */
/* EOF */



