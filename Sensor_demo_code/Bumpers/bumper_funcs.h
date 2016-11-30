/******************************************************************
*
*  bumper_funcs.h
*  version: 20161011-0           Doug Paradis
*  version: 20161129             Steve Edwards
*  Bumper functions for DPRG Club Robot 2016.
******************************************************************/
#ifndef BUMPER_FUNCS_H
#define BUMPER_FUNCS_H

#include <arduino.h>

// externs
extern const int RIGHT_BUMPER_PIN;
extern const int LEFT_BUMPER_PIN;

// func prototypes
void init_bumper();
unsigned char read_bumpers();

#endif
