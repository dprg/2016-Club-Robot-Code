/******************************************************************
*
*  claw_funcs.cpp
*  version: 20161011-0           Doug Paradis
*  Claw / Servo functions for DPRG Club Robot 2016.
*
******************************************************************/
#ifndef CLAW_FUNCS_H
#define CLAW_FUNCS_H

#include <arduino.h>
#include <SoftwareServo.h>

// externs
extern SoftwareServo claw_servo;
extern uint8_t claw_closed;

//function prototypes
void init_claw ();
void close_claw();
void open_claw();

#endif