/******************************************************************
*
*  gate_funcs.cpp
*  version: 20180504           Doug Paradis
*  gate / Servo functions for DPRG Club Robot 2016.
*
******************************************************************/
#ifndef gate_FUNCS_H
#define gate_FUNCS_H

#include <arduino.h>
#include <SoftwareServo.h>

typedef struct gate_sen_t
{
	uint8_t enable;
	uint8_t result;
	uint16_t sensor_pin;
}gate_sensor;

typedef struct gate_t
{
	uint8_t enable;
	uint8_t result;
	uint16_t servo_attach_pin;
	uint8_t open_pos;
	uint8_t closed_pos;
}grabber;

// externs
extern SoftwareServo gate_servo;
extern grabber *ptr_gate;


//function prototypes
void init_gate (grabber *ptr_gate);
void close_gate(grabber *ptr_gate);
void open_gate(grabber *ptr_gate);
void toggle_gate_state(grabber *ptr_gate);


#endif