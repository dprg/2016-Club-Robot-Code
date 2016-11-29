/******************************************************************
*
*  bumper_funcs.cpp
*  version: 20161011-0           Doug Paradis
*  Bumper functions for DPRG Club Robot 2016.
*
******************************************************************/

#include "bumper_funcs.h"

// global variables
int bump_R = 32; // Arduino pin names
int bump_L = 30;

// functions
void init_bumper()
{

    // setup pins (note: using Arduino)
    pinMode(bump_R, INPUT_PULLUP);
    pinMode(bump_L, INPUT_PULLUP);
}

uint8_t test_bumpers()
{
    uint8_t results = 0;

    if (digitalRead(bump_R) == HIGH)
    {
	results |= (0x01);
    }

    if (digitalRead(bump_L) == HIGH)
    {
	results |= (0x02);
    }
    return (results);
}
