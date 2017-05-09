/******************************************************************
*
*  claw_funcs.cpp
*  version: 20161011-0           Doug Paradis
*  Claw / Servo functions for DPRG Club Robot 2016.
*
******************************************************************/

#include "claw_funcs.h"
#include "helpful_defines.h"

// objects
SoftwareServo claw_servo; // create servo object to control a servo

// Global Variables
// servo position can be between 0 and 180, with 90 being neutral
uint8_t claw_open_pos = 15;
uint8_t claw_close_pos = 100;
uint8_t claw_closed = 0;

// functions
void init_claw()
{
    claw_servo.attach(28); // attaches the servo on Arduino pin 28 to the SoftwareServo object
    open_claw();
    claw_closed = 0;
}

void close_claw()
{
    claw_servo.write(claw_close_pos);
    claw_closed = 1;
}

void open_claw()
{
    claw_servo.write(claw_open_pos);
    claw_closed = 0;
}
