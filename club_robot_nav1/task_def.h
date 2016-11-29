/******************************************************************
*
*  task_def.h
*  version: 20161011-0           Doug Paradis
*  task definitions for DPRG Club Robot 2016.
*
******************************************************************/
#ifndef TASK_DEF_H
#define TASK_DEF_H

#include <arduino.h>
#include <task.h>
#include <log.h>
#include <sysclock.h>

#include "motor_funcs.h"
#include "wpt_funcs.h"
#include "nav_funcs.h"
#include "ultrasonic_funcs.h"
#include "bumper_funcs.h"
#include "claw_funcs.h"
#include "helpful_defines.h"

#define VERSION "BMARK-2.1.3 (clubRobot-0.0.1)"
#define WAIT(d)        \
    {                  \
        wake_after(d); \
    }

#if (MACHINE == MACH_AVR) /* Mega2560, Mega328 Teensy-LC */
#define PRINTF Serial3.println
#define SPRINTF sprintf
#endif

// function prototypes

void cpu_idle(ASIZE ignored);
void claw(ASIZE delay);
void sensors(ASIZE delay);
void move(ASIZE delay);
void stats_task(ASIZE delay);
void signon(ASIZE version);

#endif
