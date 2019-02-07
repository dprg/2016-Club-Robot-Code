/*****************************************************
*
*  sys_chks_menu_state_machine.h
*
*  author: Doug Paradis, 2017-18
*
*
******************************************************/
#ifndef _SYS_CHKS_MENU_STATE_MACHINE_H
#define _SYS_CHKS_MENU_STATE_MACHINE_H

// includes
#include "common.h"

// defines


// typedefs
typedef enum {
	SONAR_TEST,               //0 display both sonar outputs
	SCOOP_SEN_TEST,           //1 display scoop sensor state
	GATE_SERVO_TEST,          //2 display gate (claw) sensor state
	CAMERA_TEST,              //3 display target location in camera field
	ENCODERS_TEST,	          //4 display both encoder values, theta_deg (have robot turn 90 deg, pause, and return to front)
	EXIT_SYS_CHKS             //5 return to main menu
} SYS_CHKS_STATES;

// externs
extern SYS_CHKS_STATES sys_chks_state;

// func prototypes -- available to other files
SYS_CHKS_STATES sys_chks_menu_state_machine(EVENTS event);
void run_sonar_test();
void run_scoop_sen_test();
void run_gate_servo_test();
void run_camera_test();
void run_encoders_test();
void return_to_menu_fr_sys_chks();






#endif