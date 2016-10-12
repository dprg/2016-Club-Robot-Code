/******************************************************************
*
*  bumper_funcs.h
*  version: 20161011-0           Doug Paradis
*  Bumper functions for DPRG Club Robot 2016.
*
******************************************************************/
#ifndef BUMPER_FUNCS_H
#define BUMPER_FUNCS_H


#include <arduino.h>

// externs
extern int bump_R;
extern int bump_L;

// func prototypes
void init_bumper();
uint8_t test_bumpers();

#endif
