#include "common.h"
/*************************************************************************************

  16x2 LCD display with buttons program for DPRG Club Robot 2016

  This program is loaded into an arduino Uno with a 16x2 LCD display with buttons shield,
  similar to the SainSmart LCD display.
 
  Author: Doug Paradis, 2017-2018
  
  Code for shield setup taken and modified from example code written by Mark Bramwell, July 2010.

**************************************************************************************/

#include "common.h"
#include "main_menu_state_machine.h"
#include "run_menu_state_machine.h"
#include "sys_chks_menu_state_machine.h"
#include "comm_funcs.h"

// set LCD
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);           // select the pins used on the LCD panel

// global variables
EVENTS event;
STATE_MACHINE_INDICES index;
int adc_key_in  = 0;
char testing_flg = OFF;





 
// func prototypes (not needed with Arduino 
		
//EVENTS read_LCD_buttons();
//MENU_STATES main_menu_state_machine(EVENTS event);  


void setup(){
	lcd.begin(16, 2);               // start the library
	lcd.setCursor(0,0);             // set the LCD cursor   position 
	lcd.print("wait please");         // print a simple message on the LCD
	
	initize();
	 
}
 
void loop(){

	/* Read the next incoming event. Usually this is a blocking function. */
	event = read_LCD_buttons();
		
	//Serial.println(event);
	
	
	// mechanism that allows switching between state machines
	switch (index)
	{
		case MAIN_MENU_INDEX:
		menu_state = main_menu_state_machine(event);
		break;
		
		case SYS_CHKS_INDEX:
		sys_chks_state = sys_chks_menu_state_machine(event);
		break;
		
		case RUN_INDEX:
		run_state = run_menu_state_machine(event);
		break;
		
		default:
		break;
		
	}
	
	delay(10); 


	  
}


// --------------------------------- funcs ----------------------------------------------
EVENTS read_LCD_buttons(){               // read the buttons
    adc_key_in = analogRead(0);       // read the value from the sensor note: adc_key_in is global
	int current_button = (int)NO_BUTTON;
	static int last_button = (int) NO_BUTTON;
	

    // my buttons when read are centered at these values: 0, 144, 329, 504, 741
    // we add approx 50 to those values and check to see if we are close
    // We make this the 1st option for speed reasons since it will be the most likely result

    //if (adc_key_in > 1000) return NO_BUTTON;
	
	// For V1.1 usej this threshold
	/*
    if (adc_key_in < 50)   return RIGHT_BUTTON;  
    if (adc_key_in < 250)  return UP_BUTTON; 
    if (adc_key_in < 450)  return DOWN_BUTTON; 
    if (adc_key_in < 650)  return LEFT_BUTTON; 
    if (adc_key_in < 850)  return SELECT_BUTTON; 
	*/
	
	 
	if (adc_key_in < 50)   current_button = (int) RIGHT_BUTTON;
	else if (adc_key_in < 180)  current_button = (int) UP_BUTTON;
	else if (adc_key_in < 350)  current_button = (int) DOWN_BUTTON;
	else if (adc_key_in < 525)  current_button = (int)LEFT_BUTTON;
	else if (adc_key_in < 770)  current_button = (int) SELECT_BUTTON;
	else current_button = (int) NO_BUTTON;
	
	
	
	// req that NO_BUTTON occur before changing
	if ((current_button == (int)NO_BUTTON) && (last_button != (int)NO_BUTTON)) {
		current_button = last_button;
		last_button = (int) NO_BUTTON;
		return (EVENTS) current_button;		
	}
	else if ((current_button != (int)NO_BUTTON) && (last_button == (int)NO_BUTTON)) {
		last_button = current_button;
		return NO_BUTTON;
	}
	else {
		 return NO_BUTTON;
	}
	
}

void initize(){
	delay(1000);        // delay to allow the robot to settle down
	menu_state = SPLASH_SCREEN;
	run_state = WAITING;
	sys_chks_state = SONAR_TEST;
	
	index = MAIN_MENU_INDEX;
	event = NO_BUTTON;
	
	initialize_comm ();
}

void stop_running_test()
{
	EVENTS temp = NO_BUTTON;
	long start, stop;
	
	start = millis();
	stop = start + 5;
	
	while (millis() < stop) {
		temp = read_LCD_buttons();
		if (temp != NO_BUTTON) {
			testing_flg = ON;
		}
	}

}

void clear_LCD()
{
	lcd.setCursor(0,0);
	lcd.print("                ");   // clear line;
	lcd.setCursor(0,1);
	lcd.print("                ");   // clear line;
}

void clear_LCD_line(uint8_t line_num){
	lcd.setCursor(0,line_num);
	lcd.print("                ");   // clear line;
}