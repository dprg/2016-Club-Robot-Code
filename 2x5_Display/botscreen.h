///////////////////////////////////////////////////////////////////////////////
//
// DPRG Club Robot Display Drivers
// Created 6/12/2016
// Author Steve Edwards
//
///////////////////////////////////////////////////////////////////////////////
#ifndef _BOTSCREEN_H
#define _BOTSCREEN_H
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>
//#include <SD.h>                       // SD Card library -- commented out, not enough RAM available requires ~ 800 bytes RAM
#include "defines.h"
#include "bottypes.h"

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif

// Per Screen limitations
#define MAX_BUTTONS 16
#define NAV_BUTTONS 4

int BlankButtonNavFunc(char* label);

class BotScreen {

  public: //////////////////////// PUBLIC METHODS //////////////////////// 
    BotScreen(); // Constructor
    ~BotScreen();

    void Init();
    
    /* http://www.cplusplus.com/forum/beginner/18060/ use of function pointers*/
    void ShowSplashScreen(char* title, char* builder, char* robotName, int majorVer, int minorVer);

    void // Button functions
      AddButton(const ScreenButton& button),
      CheckForButtonPress(TS_Point p),
      DrawButton(const char* title),
      DrawButtons(),
      DrawBorder(uint16_t borderSize, uint16_t color);
    uint16_t GetTopButtonCount();
    uint16_t GetBottomButtonCount();
    void ClearTopButtons();
    void ClearBottomButtons();

    // in memory settings
    void InitializeSettings();
    int SaveSetting(char* key, int value, byte precision);
    int GetSetting(char* key);
    byte GetPrecision(char* key);

    // bottom status bar
    void UpdateStatus(char* status, int textSize, StatusLine line);
    void UpdateStatus(int value, int textSize, StatusLine line);

    // Touch screen read
    TS_Point waitOneTouch();

    // wrapped TFT calls
    void 
      fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
      fillScreen(uint16_t color),
      setCursor(int16_t x, int16_t y),
      setTextColor(uint16_t c),
      setTextColor(uint16_t c, uint16_t bg),
      setTextSize(uint8_t s),
      setTextWrap(boolean w),
      setRotation(uint8_t r),
      print(char* text),
      println(char* text),  
      print(int val),
      println(int val);    
    
    int16_t height(void) const,
      width(void) const;

  private: //////////////////////// PRIVATE METHODS //////////////////////// 
    // Mapped points from TS to rotated soordinates
    uint16_t mapXPoint(TS_Point p),
      mapYPoint(TS_Point p);

    //  Buttom functions
    void StartNewButtonLine();
    void AddBlankButton(int nextButton);
    void AddActionButton(const ScreenButton& button, char* label, byte textSize);
    void AddLabelButton(const ScreenButton& button);
    void AddValueButton(const ScreenButton& button);
    void AddTopButton(const ScreenButton& button);
    void AddBottomButton(const ScreenButton& button);
    
  private: //////////////////////// PRIVATE VARIABLES //////////////////////// 
    uint16_t _width, _height; // Display w/h as modified by current rotation
    byte // screen button counts
      _toggleButtons,
      _editButtons,
      _valueButtons,
      _actionButtons,
      _blankButtons,
      _navButtons;

    // last border size
    uint8_t _borderSize;
    // display libs
    Adafruit_ILI9341 *_tft;    
    Adafruit_STMPE610 *_ts;

    // meta arrays
    ButtonMeta buttonMeta[MAX_BUTTONS];
    ButtonMeta navMeta[NAV_BUTTONS];

    Setting _settings[SETTING_SLOTS];
};

#endif //_BOTSCREEN_H
