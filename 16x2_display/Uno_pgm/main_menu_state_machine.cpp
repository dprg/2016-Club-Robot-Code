/********************************************************

  Main menu menu_state machine

	SYS_CHKS_MENU,
	CONFIG_MENU,
	TEST_PATHS_MENU,
	RUN_MENU,
	EXIT_MENU                // sub-menu selected (move to other state machine)


    author: Doug Paradis, 2017-18
	
********************************************************/

#include "common.h"
#include "main_menu_state_machine.h"

MENU_STATES menu_state;

MENU_STATES main_menu_state_machine(EVENTS event)
{

	/* Switch the menu_state and the event to execute the right transition. */
	switch(menu_state)
	{
		case SPLASH_SCREEN:
		switch(event)
		{
			case LEFT_BUTTON:
			menu_state = SYS_CHKS_MENU;
			break;
			/*
			case RIGHT_BUTTON:
			menu_state = CONFIG_MENU;
			break;
			*/
			case SELECT_BUTTON:
			menu_state = SYS_CHKS_MENU;
			index = SYS_CHKS_INDEX;
			
			default:
			break;
		}
		break;
		
		case SYS_CHKS_MENU:
		switch(event)
		{
			case LEFT_BUTTON:
			menu_state = RUN_MENU;
			break;
			
			case RIGHT_BUTTON:
			menu_state = SPLASH_SCREEN;
			break;
			
			case SELECT_BUTTON:
			menu_state = SYS_CHKS_MENU;
			index = SYS_CHKS_INDEX;
			
			default:
			break;
		}
		break;
		
		case RUN_MENU:
		switch(event)
		{
			/*
			case LEFT_BUTTON:
			menu_state = TEST_PATHS_MENU;
			break;
			*/
			
			case RIGHT_BUTTON:
			menu_state = SYS_CHKS_MENU;
			break;
			
			case SELECT_BUTTON:
			menu_state = RUN_MENU;
			index = RUN_INDEX;
			break;
			
			default:
			break;
		}
		break;
		/*
		case TEST_PATHS_MENU:
		switch(event)
		{
			case LEFT_BUTTON:
			menu_state = CONFIG_MENU;
			break;
			
			case RIGHT_BUTTON:
			menu_state = RUN_MENU;
			break;
			
			case SELECT_BUTTON:
			menu_state = TEST_PATHS_MENU;
			index = TEST_PATHS_INDEX;
			break;
			
			default:
			break;
		}
		break;
		
		case CONFIG_MENU:
		switch(event)
		{
			case LEFT_BUTTON:
			menu_state = SYS_CHKS_MENU;
			break;
			
			case RIGHT_BUTTON:
			menu_state = TEST_PATHS_MENU;
			break;
			
			case SELECT_BUTTON:
			menu_state = CONFIG_MENU;
			index = CONFIG_INDEX;
			break;
			
			default:
			break;
		}
		break;
		*/
	}
	
	switch (menu_state)
	{
		case SPLASH_SCREEN:
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("DPRG Club Robot");
		lcd.setCursor(0,1);
		lcd.print("Owner: Doug P.");
		break;
		
		case SYS_CHKS_MENU:
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("System Chk Menu ");
		break;
		/*
		case CONFIG_MENU:
		lcd.setCursor(0,0);
		lcd.print("Configure Menu  ");
		break;
		case TEST_PATHS_MENU:
		lcd.setCursor(0,0);
		lcd.print("Test Paths Menu ");
		break;
		*/
		case RUN_MENU:
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Run Menu        ");
		break;
		default:
		break;
	}
	
	return(menu_state);
}