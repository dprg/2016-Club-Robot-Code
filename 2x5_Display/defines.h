///////////////////////////////////////////////////////////////////////////////
//
// DPRG Club Robot Display Drivers
// Created 6/12/2016
// Author Steve Edwards
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _DEFINES_H
#define _DEFINES_H

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS                      A3 // Chip Select goes to Analog 3
#define LCD_CD                      A2 // Command/Data goes to Analog 2
#define LCD_WR                      A1 // LCD Write goes to Analog 1
#define LCD_RD                      A0 // LCD Read goes to Analog 0
#define LCD_RESET                   A4 // Can alternately just connect to Arduino's reset pin

// For Arduino Uno/Duemilanove, etc
//  connect the SD card with DI going to pin 11, DO going to pin 12 and SCK going to pin 13 (standard)
//  Then pin 10 goes to CS (or whatever you have set up)
#define SD_CS                       10     // Set the chip select line to whatever you use (10 doesnt conflict with the library)

// Assign human-readable names to some common 16-bit color values:
#define BLACK                       0x0000
#define BLUE                        0x001F
#define RED                         0xF800
#define GREEN                       0x07E0
#define CYAN                        0x07FF
#define MAGENTA                     0xF81F
#define YELLOW                      0xFFE0
#define WHITE                       0xFFFF

// these are the default colors defined for the application
#define FOREGROUND_COLOR            BLACK
#define BACKGROUND_COLOR            WHITE
#define BORDER_COLOR                BLUE
#define BUTTON_OUTLINE              BLUE
#define BUTTON_FILL                 BLUE
#define BUTTON_TEXTCOLOR            YELLOW
#define BLANK_BUTTON                BACKGROUND_COLOR

//These are the pins for the shield!
#define YP                          A3  // must be an analog pin, use "An" notation!
#define XM                          A2  // must be an analog pin, use "An" notation!
#define YM                          9   // can be a digital pin
#define XP                          8   // can be a digital pin

// Calibrated values
#define TS_MINX                     200  //100   //200
#define TS_MINY                     118   //118
#define TS_MAXX                     950  //900   //817
#define TS_MAXY                     822   //822

// Calibrates value
#define SENSIBILITY                 300
#define MINPRESSURE                 10
#define MAXPRESSURE                 1000

// Button defines
#define XOFFSET                     46
#define YOFFSET                     20
#define BUTTON_W                    70
#define BUTTON_H                    20
#define Y_SPACING                   5
#define X_SPACING                   5
#define TEXTSIZE                    1

// Modes
#define PARENTS_BASE                000
#define PARENT_MODE_FCB             001
#define PARENT_MODE_RUNSCREEN       002
#define PARENT_MODE_ARBITRATE       003

#define INITIAL_MODE                PARENT_MODE_FCB

#define MODE_FEATURES               300
#define MODE_BEHAVIORS              400
#define MODE_RUNSCREENS             500
#define MODE_ARBITRATE              600

#define FEATURES                    300
#define FEATURE_PID_BASE            310
#define FEATURE_PID_EDIT_LEFT       311
#define FEATURE_PID_EDIT_RIGHT      312
#define FEATURE_SONAR_BASE          320
#define FEATURE_SONAR_LEFT          321
#define FEATURE_SONAR_RIGHT         322
#define FEATURE_IR_BASE             330
#define FEATURE_IR_LEFT             331
#define FEATURE_IR_RIGHT            332
#define FEATURE_ENCODERS_BASE       340
#define FEATURE_ENCODERS_LEFT       341
#define FEATURE_ENCODERS_RIGHT      342
#define FEATURE_BUMPERS_BASE        350
#define FEATURE_BUMPERS_LEFT        351
#define FEATURE_BUMPERS_RIGHT       352
#define FEATURE_PWM_BASE            360
#define FEATURE_PWM1                361

#define BEHAVIORS_BASE              400
#define BEHAVIOR_CRUISE             401
#define BEHAVIOR_FOLLOW             402
#define BEHAVIOR_AVOID              403
#define BEHAVIOR_BUMPESCAPE         404
#define BEHAVIOR_IRESCAPE           405
#define BEHAVIOR_FIND               406
#define BEHAVIOR_NAVIGATE           407

#define RUN_STATES_BASE             500
#define RUN_STATE_ENCODER           501
#define RUN_STATE_BUMPER            502
#define RUN_STATE_SONAR             503
#define RUN_STATE_IR                504

#define ARBITRATE_FREERUN_BASE      600
#define ARBITRATE_GO                601
#define ARBITRATE_GAME_BASE         610
#define ARBITRATE_6CAN              611
#define ARBITRATE_CANCAN            612
#define ARBITRATE_5SPOT             613
#define ARBITRATE_TABLETOP          614

#define ERROR_MODE_BASE             900
#define ERROR_NO_MODE_MAPPED        901

#define CNT_PARENT_MODES            3
#define CNT_PID_MODES               2
#define CNT_SONAR_MODES             2
#define CNT_IR_MODES                2
#define CNT_ENCODER_MODES           2
#define CNT_BUMPER_MODES            2
#define CNT_BEHAVIOR_MODES          7
#define CNT_RUN_STATES              4
#define CNT_ARBITRATE_FREE_MODES    1
#define CNT_ARBITRATE_GAME_MODES    4
#define CNT_ERROR_MODES             1

// Error Codes
#define SETTING_SLOTS                1
#define SETTING_NOSLOT_AVAILALBE    -32001
#define SETTING_NOTFOUND            -32000

#endif //_DEFINES_H
