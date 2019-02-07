/*****************************************************
*
*   main_menu_state_machine.h
*
*
*  author: Doug Paradis, 2017-18
*
*
******************************************************/
#ifndef _MAIN_MENU_STATE_MACHINE_H
#define _MAIN_MENU_STATE_MACHINE_H

// includes
#include "common.h"

// defines

// typedefs
typedef enum {
	SPLASH_SCREEN,
	SYS_CHKS_MENU,
	//CONFIG_MENU,
	//TEST_PATHS_MENU,
	RUN_MENU
} MENU_STATES;

// externs
extern MENU_STATES menu_state;

// func prototypes -- available to other files
MENU_STATES main_menu_state_machine(EVENTS event);







#endif