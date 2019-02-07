/******************************************************************
*
*  gate_funcs.cpp
*  version: 20180504           Doug Paradis
*  gate / Servo functions for DPRG Club Robot 2016.
*
******************************************************************/

#include "gate_funcs.h"
#include "helpful_defines.h"

// objects
SoftwareServo gate_servo;                         // create servo object to control a servo

// Global Variables

// global variables
grabber gate;
grabber *ptr_gate = &gate;


// functions
void init_gate (grabber *ptr_gate)
{
	ptr_gate->servo_attach_pin = 28;
	ptr_gate->closed_pos = 40;
	ptr_gate->open_pos = 140;
	ptr_gate->enable = ON;
	gate_servo.attach(ptr_gate->servo_attach_pin);  // attaches the servo on Arduino pin 28 to the SoftwareServo object
	open_gate(ptr_gate);                            // sets ptr_gate->result to OPEN as part of func
	
}


void close_gate(grabber *ptr_gate)
{
	gate_servo.write(ptr_gate->closed_pos);
	ptr_gate->result = CLOSE;
}

void open_gate(grabber *ptr_gate)
{
	gate_servo.write(ptr_gate->open_pos);
	ptr_gate->result = OPEN;
}

void toggle_gate_state(grabber *ptr_gate)
{
	if(ptr_gate->result == OPEN){
		close_gate(ptr_gate);
		
	}
	else{
		open_gate(ptr_gate);
	}
}

