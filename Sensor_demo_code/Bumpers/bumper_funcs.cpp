/******************************************************************
*
*  bumper_funcs.cpp
*  version: 20161011-0           Doug Paradis
*  version: 20161129             Steve Edwards
*  
*  Bumper functions for DPRG Club Robot 2016.
******************************************************************/

#include "bumper_funcs.h"

// global variables
const int RIGHT_BUMPER_PIN = 32; // Arduino pin names
const int LEFT_BUMPER_PIN  = 30;

// functions
void init_bumper()
{
    pinMode(RIGHT_BUMPER_PIN, INPUT_PULLUP);
    pinMode(LEFT_BUMPER_PIN, INPUT_PULLUP);
}

unsigned char read_bumpers()
{
    unsigned char results = 0;

    if (digitalRead(RIGHT_BUMPER_PIN) == HIGH)
    {
	    results |= (0x01);
    }

    if (digitalRead(LEFT_BUMPER_PIN) == HIGH)
    {
	    results |= (0x02);
    }
    
    return (results);
}

