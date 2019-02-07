/*****************************************************
*
*  run_menu_state_macine.h
*
*  author: Doug Paradis, 2017-18
*
*
******************************************************/
#ifndef _RUN_MENU_STATE_MACHINE_H
#define _RUN_MENU_STATE_MACHINE_H

// includes
#include "common.h"

// defines

// typedefs
typedef enum {
	WAITING,                 // robot waiting to be released
	RELEASED,                // robot released
} RUN_STATES;

// externs
extern RUN_STATES run_state;

// func prototypes -- available to other files
RUN_STATES run_menu_state_machine(EVENTS event);
void start_robot();
void update_run_data();






#endif