/******************************************************************
*
*  keyesir_funcs.h
*  version: 20161130             Steve Edwards
*  Keyes Ir Object Detection functions for DPRG Club Robot 2016.
******************************************************************/
#ifndef KEYESIR_FUNCS_H
#define KEYESIR_FUNCS_H

#include <arduino.h>

// externs
extern const int LEFT_IR_PIN;
extern const int LEFT_IR_ENABLE_PIN;
extern const int RIGHT_IR_PIN;
extern const int RIGHT_IR_ENABLE_PIN;

// func prototypes
void init_keyes_ir();
unsigned char read_keyesIr();

#endif
