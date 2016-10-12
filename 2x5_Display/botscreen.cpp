///////////////////////////////////////////////////////////////////////////////
//
// DPRG Club Robot Display Drivers
// Created 6/12/2016
// Author Steve Edwards
//
///////////////////////////////////////////////////////////////////////////////
#include "botscreen.h"
#include "defines.h"
///////////////////////////////////////////////////////////////////////////////

// Init TouchScreen: & TFT object
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, SENSIBILITY);

//
// Constructor
BotScreen::BotScreen()
{
	_tft = &tft; 
	_ts = &ts;
}

//
// Destructor
BotScreen::~BotScreen()
{
	_tft = nullptr;
	_ts = nullptr;
}

//
// Initialization
void BotScreen::Init()
{
  // initialize the touch screen
  _tft->reset();  
  _tft->begin(0x9341);
  _tft->setRotation(1); // Button Right is the initial rotation

  InitializeSettings();

  // Get initial width and height
  _width = _tft->width() - 1;
  _height = _tft->height() - 1;
}

//
// Return the size of the display (per current rotation)
int16_t BotScreen::width(void) const {
	return _width;
}

//
// Return the size of the display (per current rotation)
int16_t BotScreen::height(void) const {
	return _height;
}

//
// add a button
void BotScreen::AddButton(const ScreenButton& button)
{
  uint16_t mod, line, column, nextButton;
  char label[10];
  char value[18];
  
  switch (button.type)
  {
    case Edit:
    {
      // begin a new line
      StartNewButtonLine();

      // Label 
      AddLabelButton(button);
      
      // Value 
      AddValueButton(button);

      // - button
      AddActionButton(button, "-", 2);
      
      // + Button
      AddActionButton(button, "+", 2);
        
      break;
    }
    case Value:
    {
      // stat a new line of buttons
      StartNewButtonLine();

      // Label 
      AddLabelButton(button);

      // Value 
      AddValueButton(button);

      // blank
      AddBlankButton(GetTopButtonCount());

      // READ button
      AddActionButton(button, "Read", 1);

      break;
    }
    default:
    {
      if(button.placement == Top)
      {
        AddTopButton(button);
      } else { // BOTTOM
        AddBottomButton(button);
      }
      break;
    }
  }
}

void BotScreen::StartNewButtonLine()
{
  uint16_t mod, nextButton;
  nextButton = GetTopButtonCount();
  mod = nextButton % 4;
  
  if(mod > 0) {
    for(int b=nextButton;b<(nextButton+(4-mod));b++)
    {
      AddBlankButton(b);
    }
  }  
}

void BotScreen::AddBlankButton(int nextButton)
{
  buttonMeta[nextButton].screen_button.initButton(_tft,
        0, 0, 0, 0,                                 // x, y, w, h
        BLANK_BUTTON, BLANK_BUTTON, BLANK_BUTTON,   // outline, fill, textcolor
        "blank", TEXTSIZE);                         // label, text size
        _blankButtons++;
  
  buttonMeta[nextButton].buttonType = Blank;
  buttonMeta[nextButton].buttonFcb = false;
  buttonMeta[nextButton].navFunc = BlankButtonNavFunc;
}

void BotScreen::AddLabelButton(const ScreenButton& button)
{
  uint16_t mod, line, column, nextButton;
  char label[10];
  
  nextButton = GetTopButtonCount();
  mod = nextButton % 4;
  line = abs(nextButton/4)+1;
  column = mod;
  memset(label, '\0', sizeof(label));
  strncpy(label, button.label, sizeof(button.label));
  
  // blank button, setting label
  buttonMeta[nextButton].screen_button.initButton(_tft,
            (column *(BUTTON_W + X_SPACING)) + XOFFSET,   // x
            (line * (BUTTON_H + Y_SPACING)) + YOFFSET,    // y
            BUTTON_W, BUTTON_H,                           // w, h
            BLANK_BUTTON, BLANK_BUTTON, BLACK,            // outline, fill, textcolor
            label, TEXTSIZE + 1);                         // label, text size
  _editButtons++;

  buttonMeta[nextButton].value = 0;
  buttonMeta[nextButton].buttonType = Value;
  buttonMeta[nextButton].buttonFcb = false;
  buttonMeta[nextButton].navFunc = BlankButtonNavFunc;  
}

void BotScreen::AddValueButton(const ScreenButton& button)
{
  uint16_t mod, line, column, nextButton;
  char value[18];

  nextButton = GetTopButtonCount();
  mod = nextButton % 4;
  line = abs(nextButton/4)+1;
  column = mod;
  memset(value, '\0', sizeof(value));
  itoa(button.value, value, 10);
  
  // blank button, setting value
  buttonMeta[nextButton].screen_button.initButton(_tft,
            (column *(BUTTON_W + X_SPACING)) + XOFFSET,   // x
            (line * (BUTTON_H + Y_SPACING)) + YOFFSET,    // y
            BUTTON_W, BUTTON_H,                           // w, h
            BLANK_BUTTON, BLANK_BUTTON, BLACK,            // outline, fill, textcolor
            value, TEXTSIZE + 1);                         // label, text size
  _valueButtons++;
  
  buttonMeta[nextButton].value = button.value;
  buttonMeta[nextButton].buttonType = Value;
  buttonMeta[nextButton].buttonFcb = false;
  buttonMeta[nextButton].navFunc = BlankButtonNavFunc;  
}

void BotScreen::AddActionButton(const ScreenButton& button, char* label, byte textSize)
{
  uint16_t mod, line, column, nextButton;
  char value[18];
  
  nextButton = GetTopButtonCount();
  mod = nextButton % 4;
  line = abs(nextButton/4)+1;
  column = mod;
  
  buttonMeta[nextButton].screen_button.initButton(_tft,
            (column *(BUTTON_W + X_SPACING)) + XOFFSET,     // x
            (line * (BUTTON_H + Y_SPACING)) + YOFFSET,      // y
            BUTTON_W, BUTTON_H,                             // w, h
            BUTTON_OUTLINE, BUTTON_FILL, BUTTON_TEXTCOLOR,  // outline, fill, textcolor
            label, textSize);                                 // label, text size
  _actionButtons++;
  
  buttonMeta[nextButton].value = 0;
  buttonMeta[nextButton].buttonType = Edit;
  buttonMeta[nextButton].buttonFcb = false;
  buttonMeta[nextButton].navFunc = button.navFunc;  
}
//
// Add a button to top of screen
void BotScreen::AddTopButton(const ScreenButton& button)
{
  uint16_t mod, line, column, nextButton;
  char label[10];
  char value[18];

  nextButton = GetTopButtonCount();
  
  mod = nextButton % 4;
  line = abs(nextButton/4)+1;
  column = mod;
  memset(label, '\0', sizeof(label));
  strncpy(label, button.label, sizeof(button.label));
  
  buttonMeta[nextButton].screen_button.initButton(_tft,
            (column *(BUTTON_W + X_SPACING)) + XOFFSET,     // x
            (line * (BUTTON_H + Y_SPACING)) + YOFFSET,      // y
            BUTTON_W, BUTTON_H,                             // w, h
            BUTTON_OUTLINE, BUTTON_FILL, BUTTON_TEXTCOLOR,  // outline, fill, textcolor
            label, TEXTSIZE);                               // label, text size
  _toggleButtons++;

  buttonMeta[nextButton].buttonType = button.type;
  buttonMeta[nextButton].buttonFcb = button.fcb;
  buttonMeta[nextButton].navFunc = button.navFunc;
}

//
// Add button to bottom of screen
void BotScreen::AddBottomButton(const ScreenButton& button)
{
  uint16_t mod, line, column, nextButton;
  char label[10];
  char value[18];
    
  nextButton = GetBottomButtonCount();
  
  mod = nextButton % 4;
  line = abs(nextButton/4)+1;
  column = mod;
  memset(label, '\0', sizeof(label));
  strncpy(label, button.label, sizeof(button.label));
  navMeta[nextButton].screen_button.initButton(_tft,
            (column *(BUTTON_W + X_SPACING)) + XOFFSET,           // x
            _height -((line * (BUTTON_H + Y_SPACING)) + YOFFSET), // y
            BUTTON_W, BUTTON_H,                                   // w, h
            BUTTON_OUTLINE, BUTTON_FILL, BUTTON_TEXTCOLOR,        // outline, fill, textcolor
            label, TEXTSIZE);                                     // label, text size
  _navButtons++;

  navMeta[nextButton].buttonType = button.type;
  navMeta[nextButton].buttonFcb = button.fcb;
  navMeta[nextButton].navFunc = button.navFunc;  
}

//
// Check each button to see if it was pressed
void BotScreen::CheckForButtonPress(TSPoint p)
{
  // map to current rotation coordinates
  uint16_t x = mapXPoint(p);
  uint16_t y = mapYPoint(p);
  
  // iterate through all top buttons
  for (uint8_t b=0; b<GetTopButtonCount(); b++) {
    if (buttonMeta[b].screen_button.contains(x, y)) {

      /////////////////////////////////////////////// TOGGLE ///////////////////////////////////////////////
      if (buttonMeta[b].buttonType == Toggle) {
        buttonMeta[b].screen_button.press(true);

        if(buttonMeta[b].navFunc != nullptr) {
          buttonMeta[b].buttonFcb = buttonMeta[b].navFunc(buttonMeta[b].screen_button.getLabel());
        }
        buttonMeta[b].screen_button.drawButton(buttonMeta[b].buttonFcb);
        
      /////////////////////////////////////////////// EDIT ///////////////////////////////////////////////
      } else if(buttonMeta[b].buttonType == Edit) {
        buttonMeta[b].screen_button.press(true);
        char valBuffer[10];
        char* value = valBuffer;

        switch ((b % 4))
        {
          case 2: // addition button
            if(buttonMeta[b].navFunc != nullptr) {
              buttonMeta[b-1].value = buttonMeta[b].navFunc(buttonMeta[b].screen_button.getLabel());
            }
            itoa(buttonMeta[b-1].value, valBuffer,10);
            buttonMeta[b-1].screen_button.setLabel(value, sizeof(valBuffer));
            buttonMeta[b-1].screen_button.drawButton(true);
          break;
          case 3: // subtract button
            if(buttonMeta[b].navFunc != nullptr) {
              buttonMeta[b-2].value = buttonMeta[b].navFunc(buttonMeta[b].screen_button.getLabel());
            }
            itoa(buttonMeta[b-2].value, valBuffer,10);
            buttonMeta[b-2].screen_button.setLabel(value, sizeof(valBuffer));
            buttonMeta[b-2].screen_button.drawButton(true);
          break;
        }

        buttonMeta[b].screen_button.drawButton(true);
        delay(50);
        buttonMeta[b].screen_button.drawButton(false);
        
      /////////////////////////////////////////////// VALUE ///////////////////////////////////////////////
      } else if(buttonMeta[b].buttonType == Value) {

        
      /////////////////////////////////////////////// ACTION ///////////////////////////////////////////////
      } else if(buttonMeta[b].buttonType == Action) {
        buttonMeta[b].screen_button.press(true);
        buttonMeta[b].screen_button.drawButton(true);
        delay(150);
        buttonMeta[b].screen_button.drawButton(false);

      /////////////////////////////////////////////// NAVIGATION ///////////////////////////////////////////////
      } else if(buttonMeta[b].buttonType == Navigation) {
        buttonMeta[b].screen_button.press(true);
        buttonMeta[b].screen_button.drawButton(true);
        delay(150);
        buttonMeta[b].screen_button.drawButton(false);

        if(buttonMeta[b].navFunc != nullptr) {
          buttonMeta[b].navFunc(buttonMeta[b].screen_button.getLabel());
        }
        
        buttonMeta[b].buttonFcb = !buttonMeta[b].buttonFcb;
        
      /////////////////////////////////////////////// BLANK ///////////////////////////////////////////////
      } else if(buttonMeta[b].buttonType == Blank) {
        
      }
    }
  }

  // Check to see if our Navigation buttons were pressed
  for (uint8_t b=0; b<GetBottomButtonCount(); b++) {
    if (navMeta[b].screen_button.contains(x, y)) {
      if (navMeta[b].buttonType == Toggle) {
        navMeta[b].screen_button.press(true);
        navMeta[b].buttonFcb = !navMeta[b].buttonFcb;
        if(navMeta[b].navFunc != nullptr) {
          navMeta[b].navFunc(navMeta[b].screen_button.getLabel());
        }
        navMeta[b].screen_button.drawButton(navMeta[b].buttonFcb);      
      } else if (navMeta[b].buttonType == Navigation) {
        navMeta[b].screen_button.press(true);
                    
        if(navMeta[b].navFunc != nullptr) {
          navMeta[b].navFunc(navMeta[b].screen_button.getLabel());
        }
        
        navMeta[b].screen_button.drawButton(true);
        delay(350);
        navMeta[b].screen_button.drawButton(false);
      }
    }
  }    
}

//
// redraw the specified button
void BotScreen::DrawButton(const char* title)
{
  for(uint16_t b=0;b<GetTopButtonCount();b++) {
    if(buttonMeta[b].screen_button.getLabel() == title)
    {
      buttonMeta[b].screen_button.drawButton(buttonMeta[b].buttonFcb);
      break;
    }
  }  
}

//
// redraw all the currently defined buttons
void BotScreen::DrawButtons()
{                       
  for(uint16_t b=0;b<GetTopButtonCount();b++) {
    buttonMeta[b].screen_button.drawButton(buttonMeta[b].buttonFcb);
  }	
  for(uint16_t b=0;b<GetBottomButtonCount();b++) {
    navMeta[b].screen_button.drawButton(navMeta[b].buttonFcb);
  }
}

//
// initialize our settings array
bool BotScreen::InitializeSettings()
{
  for(int i=0;i<SETTING_SLOTS;i++) {
    memset(_settings[i].key, '\0', 10);
    _settings[i].value = 0;
    _settings[i].precision = 0;
  }  
}

//
// save or update a key/value pair in next available slot
int BotScreen::SaveSetting(char* key, int value, byte precision)
{
  // update existing
  for(int i=0;i<SETTING_SLOTS;i++) {
    if(strcmp(_settings[i].key, key) == 0) {
      _settings[i].value = value;
      _settings[i].precision = precision;
      return _settings[i].value;
    }
  }

  // create new setting
  uint8_t freePos = -1;
  for(int i=0;i<SETTING_SLOTS;i++) {
    if(strcmp(key, "") == 0) {
      strncpy(_settings[i].key, key, 10);      
      _settings[i].value = value;
      _settings[i].precision = precision;
      return _settings[i].value;
    }
  }

  int i = 1/0;
  return SETTING_NOSLOT_AVAILALBE;
}

//
// get precision value to tell us how many decimals this value should have
// i.e. value = 12345 with precision = 2 would mean user saved this value 123.45
byte BotScreen::GetPrecision(char* key)
{
  for(int i=0;i<SETTING_SLOTS;i++) {
    if(strcmp(key, _settings[i].key) == 0) {
      return _settings[i].precision;
    }
  }
  return SETTING_NOTFOUND; 
}

//
// get setting referered to by key
int BotScreen::GetSetting(char* key)
{
  for(int i=0;i<10;i++) {
    if(strcmp(_settings[i].key, key) == 0) {
      return _settings[i].value;
    }
  }
  return SETTING_NOTFOUND; 
}
    
//
// get top button count
uint16_t BotScreen::GetTopButtonCount()
{
  return ( _toggleButtons
         + _editButtons
         + _valueButtons
         + _actionButtons
         + _blankButtons );
}

//
// get bottom button count
uint16_t BotScreen::GetBottomButtonCount()
{
  return _navButtons;  
}

//
// clear top buttons
void BotScreen::ClearTopButtons()
{
  _toggleButtons = 0;
  _editButtons = 0;
  _valueButtons = 0;
  _actionButtons = 0;  
  _blankButtons = 0;
}

//
// clear bottom buttons
void BotScreen::ClearBottomButtons()
{
  _navButtons = 0;
}

//
// clear screen and draw a border of specified size and color
void BotScreen::DrawBorder(uint16_t borderSize, uint16_t color)
{
  _borderSize = borderSize;
  _tft->fillScreen(color);
  _tft->fillRect(_borderSize, _borderSize, (_width - _borderSize * 2), (_height - _borderSize * 2), BACKGROUND_COLOR);    
}

//
// wait for user touch on screen
TSPoint BotScreen::waitOneTouch()
{
	TSPoint p;
	
	do {
		p= _ts->getPoint(); 
	
		pinMode(XM, OUTPUT); //Pins configures again for TFT control
		pinMode(YP, OUTPUT);
	
	} while((p.z < MINPRESSURE )|| (p.z > MAXPRESSURE));
	
	return p;
}

//
// pass through to fillRect
void BotScreen::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  _tft->fillRect(x, y, w, h, color);
}

//
// pass through to fillScreen
void BotScreen::fillScreen(uint16_t color)
{
  _tft->fillScreen(color);
}

//
// pass through to setCursor
void BotScreen::setCursor(int16_t x, int16_t y)
{
	_tft->setCursor(x, y);
}

//
// pass through to setTextColor
void BotScreen::setTextColor(uint16_t c)
{
	_tft->setTextColor(c);
}

//
// pass through to setTextColor
void BotScreen::setTextColor(uint16_t c, uint16_t bg)
{
	_tft->setTextColor(c, bg);
}

//
// pass through to setTextSize
void BotScreen::setTextSize(uint8_t s)
{
	_tft->setTextSize(s);
}

//
// pass through to setTextWrap
void BotScreen::setTextWrap(boolean w)
{
	_tft->setTextWrap(w);
}

//
// pass through to setRotation
void BotScreen::setRotation(uint8_t r)
{
	_tft->setRotation(r);
}

//
// pass through to print
void BotScreen::print(char* text)
{
  _tft->print(text);
}

//
// pass through to println
void BotScreen::println(char* text)
{
	_tft->println(text);
}

//
// pass through to print
void BotScreen::print(int val)
{
  _tft->print(val);
}

//
// pass through to println
void BotScreen::println(int val)
{
  _tft->println(val);
}

//
// Map the coordinate X  
uint16_t BotScreen::mapXPoint(TSPoint p)
{
  uint16_t x = map(p.y, TS_MINY, TS_MAXY, 0, _width);
  return x;
}

//
// Map the coordinate Y
uint16_t BotScreen::mapYPoint(TSPoint p) 
{
  uint16_t y = map(p.x, TS_MINX, TS_MAXX, 0, _height);
  return y;
}

//
// Show the splash screen
void BotScreen::ShowSplashScreen(char* title, char* builder, char* robotName, int majorVer, int minorVer)
{
  DrawBorder(5, YELLOW);
  _tft->setTextColor(FOREGROUND_COLOR);
  _tft->setCursor (15, 50);
  _tft->setTextSize (3);
  _tft->println(title);
  
  _tft->setCursor (35, 100);
  _tft->setTextSize (2);
  _tft->print("Version: ");tft.print(majorVer);tft.print(".");tft.println(minorVer);

  _tft->setCursor (35, 125);
  _tft->setTextSize (2);
  _tft->print("Builder: ");tft.println(builder);

  _tft->setCursor (35, 150);
  _tft->setTextSize (2);
  _tft->print("Robot  : ");tft.println(robotName);

  _tft->setCursor (65, 200);
  _tft->setTextSize (1);
  _tft->println("Touch to proceed");

  waitOneTouch();
}

//
// Update the status line
void BotScreen::UpdateStatus(char* status, int textSize, StatusLine line)
{
  _tft->setTextColor(FOREGROUND_COLOR);

  if(line == Line1) {
    _tft->fillRect(_borderSize+5, 210, _width -(_borderSize*2) -5, 15, BACKGROUND_COLOR);
    _tft->setCursor(_borderSize+5, 210);
  } else {
    _tft->fillRect(_borderSize+5, 225, _width -(_borderSize*2) -5, 15, BACKGROUND_COLOR);
    _tft->setCursor(_borderSize+5, 225);
  }
  
  _tft->setTextSize(textSize);
  _tft->println(status);
}

//
// Update the status line
void BotScreen::UpdateStatus(int value, int textSize, StatusLine line)
{
  char strbuf[10];
  itoa(value, strbuf, 10); 

  UpdateStatus(strbuf, textSize, line);
}

//
// global stubbed method
int BlankButtonNavFunc(char* label)
{
  // do nothing
}

