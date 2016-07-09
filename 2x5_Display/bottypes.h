///////////////////////////////////////////////////////////////////////////////
//
// DPRG Club Robot Display Drivers
// Created 6/12/2016
// Author Steve Edwards
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _BOTTYPES_H
#define _BOTTYPES_H

typedef enum {
    Toggle,
    Edit,
    Value,
    Navigation,
    Action,
    Blank
} ButtonType;

typedef enum {
    Top,
    Bottom
} ButtonPlacement;

typedef enum {
  Line1,
  Line2
} StatusLine;

typedef enum {
  Fcb_Editor,
  Home_Screens,
  Arbitrate
} DisplayMode;

// function pointer definitions
typedef int (*NavPtr)(char* label);
typedef void (*ModePtr)();

// mode map structure
struct ModeExecutionMap {
  uint16_t mode;
  ModePtr modeFunction;
};

struct Setting {
  char key[10];
  int value; 
  byte precision;
};

struct ScreenButton {
  char label[10];
  uint16_t value;
  NavPtr navFunc;
  ButtonType type;
  ButtonPlacement placement;
  bool fcb;
};

struct ButtonSetup {
  char label[10];
  ButtonType buttonType;
  bool enabled;
  NavPtr navFunc;
  uint16_t value;
};

struct ButtonMeta {
    Adafruit_GFX_Button screen_button;
    ButtonType buttonType;
    NavPtr navFunc;
    bool buttonFcb;  
    uint16_t value;
};

#endif //_BOTTYPES_H
