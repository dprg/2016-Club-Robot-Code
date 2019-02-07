/*****************************************************
*  common.h
*  menu typedefs
*
*  author: Doug Paradis, 2017-18
*
*
*
******************************************************/

#ifndef _COMMON_H
#define _COMMON_H

#include <Arduino.h>
#include <LiquidCrystal.h>


//defines
#ifndef OPEN
#define OPEN 0
#endif

#ifndef CLOSED
#define CLOSED 1
#endif

#ifndef OFF
#define OFF 0
#endif

#ifndef ON
#define ON 1
#endif

#ifndef TOP
#define TOP 0
#endif

#ifndef BOTTOM
#define BOTTOM 1
#endif

#ifndef NOT_DONE
#define NOT_DONE 0
#endif

#ifndef DONE
#define DONE 1
#endif

// typedefs
typedef enum {
	LEFT_BUTTON,
	RIGHT_BUTTON,
	SELECT_BUTTON,
	UP_BUTTON,
	DOWN_BUTTON,
	NO_BUTTON
} EVENTS;

typedef enum {
	MAIN_MENU_INDEX,        
	SYS_CHKS_INDEX,         
	//CONFIG_INDEX,
	//TEST_PATHS_INDEX,
	RUN_INDEX              
} STATE_MACHINE_INDICES;


EVENTS read_LCD_buttons();
void initize();

extern EVENTS event;
extern STATE_MACHINE_INDICES index;
extern LiquidCrystal lcd;
extern char testing_flg;

// function prototypes
void stop_running_test();
void clear_LCD();
void clear_LCD_line(uint8_t line_num);

#endif

