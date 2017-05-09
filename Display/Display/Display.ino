///////////////////////////////////////////////////////////////////////////////
//
// DPRG Club Robot Display Drivers
// Created 6/12/2016
// Author Steve Edwards
//
///////////////////////////////////////////////////////////////////////////////
#include <SPFD5408_TouchScreen.h>     // Touch library
#include "defines.h"
#include "botscreen.h"                // DPRG Bot Screen Library
///////////////////////////////////////////////////////////////////////////////

//
// Display version information (update at least minor version# if you make changes)
uint16_t major_version = 1;
uint16_t minor_version = 0;

// Mode variables
uint16_t mode = 0;
uint16_t last_mode = -1;

// Serial communication
ConnectionState commState = Disconnected;
//char data[INPUT_SIZE + 1];

//
//BotScreen object
BotScreen touchDisplay = BotScreen();

// mapped function pointer declarations
void parent_fcb();
void parent_runscreen();
void parant_arbitrate();
void pid_editor_left();
void pid_editor_right();
void sonar_editor_left();
void sonar_editor_right();
void error_no_mode_mapped();

// Make sure this # corresponds to the number of modes mapped below
uint8_t const TOTAL_MODES = 8;

ModeExecutionMap modelist[]  {
    { ERROR_NO_MODE_MAPPED, error_no_mode_mapped },
    { PARENT_MODE_FCB, parent_fcb },
    { PARENT_MODE_RUNSCREEN, parent_runscreen },
    { PARENT_MODE_ARBITRATE, parant_arbitrate },
    { FEATURE_PID_EDIT_LEFT, pid_editor_left },
    { FEATURE_PID_EDIT_RIGHT, pid_editor_right },
    { FEATURE_SONAR_LEFT, sonar_editor_left },
    { FEATURE_SONAR_RIGHT, sonar_editor_right }
  };

// Settings State
int saveresult = 0;

struct PidTerms{
  uint16_t K;
  uint16_t P;
  uint16_t I;
  uint16_t D;
};

struct SonarSetting {
  byte low;
  uint16_t high;
  bool enabled;
  uint16_t reading;
};

PidTerms pids[2];
bool fcbPidLeft;
bool fcbPidRight;
SonarSetting sonarLeft;
SonarSetting sonarRight;

//
// Initialization
void setup() 
{
  Serial.begin(9600);
  touchDisplay.Init();
  
  // ready to GO!  
  touchDisplay.ShowSplashScreen("DPRG Robot 2016", "Steve Edwards", "Havoc", major_version, minor_version);  
  saveresult = touchDisplay.SaveSetting("PID_EN", false, 0);

  pids[0].K = 200;
  pids[0].P = 1234;
  pids[0].I = 23;
  pids[0].D = 0;
  
  pids[1].K = 200;
  pids[1].P = 1234;
  pids[1].I = 23;
  pids[1].D = 0;

  fcbPidLeft = true;
  fcbPidRight = true;
  
  sonarLeft.low = 2;
  sonarLeft.high = 75;
  sonarLeft.enabled = true;
  sonarLeft.reading = 55;
  
  sonarRight.low = 2;
  sonarRight.high = 75;
  sonarRight.enabled = true;
  sonarRight.reading = 55;
  
  mode = INITIAL_MODE;

}

//
// Main Display loop
void loop() 
{ 
  // if mode has changed clear the screen and redraw border
  if(mode != last_mode)
  {    
    touchDisplay.DrawBorder(5, BLUE);
  }

  //update last_mode
  last_mode = mode;

  // Run the current screen, dictated by 'mode'
  for(int m=0;m<TOTAL_MODES;m++) {
    if(mode == modelist[m].mode) {
      modelist[m].modeFunction();
      break;
    } else {
      if(m == (TOTAL_MODES-1)){
        mode = ERROR_NO_MODE_MAPPED;
      }
    }    
  }
  touchDisplay.UpdateStatus("Mode", 1, Line1);  
  touchDisplay.UpdateStatus(mode, 1, Line2);
}

int nextMode(uint16_t BASEMODE, uint8_t MODE_CNT)
{
  last_mode = mode;
  if (mode < BASEMODE + MODE_CNT) {
    mode++;
  } else {
    mode = BASEMODE+1;
  } 
  return mode;
}

void error_no_mode_mapped()
{
    // Print Title
    touchDisplay.fillScreen(RED);
    touchDisplay.setCursor(15,100);
    touchDisplay.setTextColor(BLACK);
    touchDisplay.setTextSize(2);
    touchDisplay.println("ERROR: NO_MODE_MAPPED");

    touchDisplay.ClearTopButtons();
    touchDisplay.ClearBottomButtons();
    
    #define PID_NAV_CNT 1
    ButtonSetup navButtons[PID_NAV_CNT] { 
      {"Back", Navigation, false, nav_return_to_initial_mode} 
    };
 
    for(uint16_t b;b<PID_NAV_CNT;b++) {
      ScreenButton button;
      strncpy(button.label, navButtons[b].label, sizeof(navButtons[b].label));
      button.type = navButtons[b].buttonType;
      button.placement = Bottom;
      button.fcb = navButtons[b].enabled;
      button.navFunc = navButtons[b].navFunc;
  
      touchDisplay.AddButton(button);
    }

    touchDisplay.DrawButtons();
    
    while (mode == ERROR_NO_MODE_MAPPED)
    {
      TSPoint p;
  
      delay(250); //debounce
      p = touchDisplay.waitOneTouch(); // Wait touch
  
      if (p.y < (TS_MINY-5)) {
        nextMode(PARENTS_BASE, CNT_PARENT_MODES);
      }
      
      touchDisplay.CheckForButtonPress(p);
    }

}

void BuildScreen(char* title, ButtonSetup *topButtons, byte topButtonCount, ButtonSetup *navigationButtons, byte navButtonCount, uint16_t currentMode)
{
  touchDisplay.ClearTopButtons();
  touchDisplay.ClearBottomButtons();

  // Print Title
  touchDisplay.fillRect(15, 10, 300, 15, BACKGROUND_COLOR);
  touchDisplay.setCursor(15,10);
  touchDisplay.setTextColor(BLACK);
  touchDisplay.setTextSize (2);
  touchDisplay.println(title);

  // top buttons
  if(topButtons != nullptr) {
    for(uint16_t b;b<topButtonCount;b++) {
      ScreenButton button;
      strncpy(button.label, topButtons[b].label, sizeof(topButtons[b].label));
      button.type = topButtons[b].buttonType;
      button.placement = Top;
      button.fcb = topButtons[b].enabled;
      button.navFunc = topButtons[b].navFunc;
      button.value = topButtons[b].value;
  
      touchDisplay.AddButton(button);
    }   
  }
  
  // bottom buttons
  if(navigationButtons != nullptr) {
    for(uint16_t b;b<navButtonCount;b++) {
      ScreenButton button;
      strncpy(button.label, navigationButtons[b].label, sizeof(navigationButtons[b].label));
      button.type = navigationButtons[b].buttonType;
      button.placement = Bottom;
      button.fcb = navigationButtons[b].enabled;
      button.navFunc = navigationButtons[b].navFunc;
      button.value = topButtons[b].value;
  
      touchDisplay.AddButton(button);
    }
  }

  touchDisplay.DrawButtons();

  touchDisplay.UpdateStatus("Mode", 1, Line1);
  touchDisplay.UpdateStatus(mode, 1, Line2);
  
  checkButtonPressed(currentMode);
}

// executes CheckForButtonPress(p) unless parent mode change is requested
void checkButtonPressed(uint16_t currentMode)
{
  while (mode == currentMode)
  {
    TSPoint p;
  
    delay(250); //debounce
    p = touchDisplay.waitOneTouch(); // Wait touch
  
    if (p.y < (TS_MINY-5)) {
      if(currentMode > PARENTS_BASE + CNT_PARENT_MODES) { mode = INITIAL_MODE; }
      nextMode(PARENTS_BASE, CNT_PARENT_MODES);
    }
    
    touchDisplay.CheckForButtonPress(p);
  }
}

//
// Feature Control Bit editor display mode
void parent_fcb()
{
    //Feature Settings Editors
    #define FCB_BTN_CNT 12
    ButtonSetup fcbButtons[FCB_BTN_CNT] { 
      {"PID", Toggle, fcbPidLeft || fcbPidRight, fcb_settings_mode, 0}, 
      {"Sonar", Toggle, sonarLeft.enabled || sonarRight.enabled, fcb_settings_mode, 0}, 
      {"IR", Toggle, false, fcb_settings_mode, 0}, 
      {"Encoders", Toggle, false, fcb_settings_mode, 0}, 
      {"PWM", Toggle, false, fcb_settings_mode, 0}, 
      {"Cruise", Toggle, false, fcb_settings_mode, 0}, 
      {"Follow", Toggle, false, fcb_settings_mode, 0}, 
      {"Avoid", Toggle, false, fcb_settings_mode, 0}, 
      {"Bump Esc", Toggle, false, fcb_settings_mode, 0}, 
      {"Ir Esc", Toggle, false, fcb_settings_mode, 0}, 
      {"Find", Toggle, false, fcb_settings_mode, 0}, 
      {"Navigate", Toggle, false, fcb_settings_mode, 0}
    };
     
    #define PID_NAV_CNT 4
    ButtonSetup navButtons[PID_NAV_CNT] { 
      {"Load", Navigation, false, nav_not_implemented_callback, 0}, 
      {"Save", Navigation, false, nav_not_implemented_callback, 0}, 
      {"Defaults", Navigation, false, nav_not_implemented_callback, 0}, 
      {"Reset", Navigation, false, nav_not_implemented_callback, 0} 
    };

    char* title = "Settings";
    BuildScreen(title, fcbButtons, FCB_BTN_CNT, navButtons, PID_NAV_CNT, PARENT_MODE_FCB);
}

// msp fcb buttons to modes
int fcb_settings_mode(char* label)
{
  if(strcmp(label, "PID") == 0) {
    mode = FEATURE_PID_EDIT_LEFT;
  } else if(strcmp(label, "Sonar") == 0) {
    mode = FEATURE_SONAR_LEFT;
  } else if(strcmp(label, "IR") == 0) {
    mode = FEATURE_IR_LEFT;
  } else if(strcmp(label, "Encoders") == 0) {
    mode = FEATURE_ENCODERS_LEFT;
  } else if(strcmp(label, "PWM") == 0) {
    mode = FEATURE_PWM1;
  } else if(strcmp(label, "Cruise") == 0) {
    mode = BEHAVIOR_CRUISE;
  } else if(strcmp(label, "Follow") == 0) {
    mode = BEHAVIOR_FOLLOW;
  } else if(strcmp(label, "Avoid") == 0) {
    mode = BEHAVIOR_AVOID;
  } else if(strcmp(label, "Bump Esc") == 0) {
    mode = BEHAVIOR_BUMPESCAPE;
  } else if(strcmp(label, "Ir Esc") == 0) {
    mode = BEHAVIOR_IRESCAPE;
  } else if(strcmp(label, "Find") == 0) {
    mode = BEHAVIOR_FIND;
  } else if(strcmp(label, "Navigate") == 0) {
    mode = BEHAVIOR_NAVIGATE;
  }  

  return mode;
}

int nav_return_to_initial_mode(char* label)
{
  mode = INITIAL_MODE;
  return mode;
}

int fcb_edit_back(char* label)
{  
  mode = PARENT_MODE_FCB;
  return mode;
}

// called by fcb nav buttons - yet to be implemented
int nav_not_implemented_callback(char* label) {}

void pid_editor_left() {
  char* title = "Settings - PID Left";

  #define TOP_BTN_CNT 4
  ButtonSetup topButtons[TOP_BTN_CNT] { 
    {"K", Edit, false, leftKedit, pids[0].K}, 
    {"P", Edit, false, leftPedit, pids[0].P}, 
    {"I", Edit, true, leftIedit, pids[0].I}, 
    {"D", Edit, true, leftDedit, pids[0].D} 
  };
  
  #define NAV_BTN_CNT 4
  ButtonSetup navButtons[PID_NAV_CNT] { 
    {"Back", Navigation, false, fcb_edit_back}, 
    {"Enable", Toggle, fcbPidLeft, LeftPidToggleEnable}, 
    {"Previous", Navigation, false, nextPidMode}, 
    {"Next", Navigation, false, nextPidMode} 
  };

  // build screen for PID EDIT LEFT
  BuildScreen(title, topButtons, TOP_BTN_CNT, navButtons, NAV_BTN_CNT, FEATURE_PID_EDIT_LEFT); 
}

void pid_editor_right() {
  char* title = "Settings - PID Right";

  #define TOP_BTN_CNT 4
  ButtonSetup topButtons[TOP_BTN_CNT] { 
    {"K", Edit, false, rightKedit, pids[1].K}, 
    {"P", Edit, false, rightPedit, pids[1].P}, 
    {"I", Edit, true, rightIedit, pids[1].I}, 
    {"D", Edit, true, rightDedit, pids[1].D} 
  };
  
  #define NAV_BTN_CNT 4
  ButtonSetup navButtons[PID_NAV_CNT] { 
    {"Back", Navigation, false, fcb_edit_back}, 
    {"Enable", Toggle, fcbPidRight, RightPidToggleEnable}, 
    {"Previous", Navigation, false, nextPidMode}, 
    {"Next", Navigation, false, nextPidMode} 
  };

  // build screen for PID EDIT RIGHT
  BuildScreen(title, topButtons, TOP_BTN_CNT, navButtons, NAV_BTN_CNT, FEATURE_PID_EDIT_RIGHT);
}

void sonar_editor_left() {
  char* title = "Settings - Sonar Left";

  #define TOP_BTN_CNT 3
  ButtonSetup topButtons[TOP_BTN_CNT] { 
    {"Low", Edit, false, sonar_left_low_edit, sonarLeft.low}, 
    {"High", Edit, false, sonar_left_high_edit, sonarLeft.high}, 
    {"Ping", Value, true, nullptr, sonarLeft.reading}
  };
  
  #define NAV_BTN_CNT 4
  ButtonSetup navButtons[PID_NAV_CNT] { 
    {"Back", Navigation, false, fcb_edit_back}, 
    {"Enable", Toggle, sonarLeft.enabled, sonar_left_enable}, 
    {"Previous", Navigation, false, sonar_next_mode}, 
    {"Next", Navigation, false, sonar_next_mode} 
  };

  // build screen for PID EDIT LEFT
  BuildScreen(title, topButtons, TOP_BTN_CNT, navButtons, NAV_BTN_CNT, FEATURE_SONAR_LEFT); 
}

void sonar_editor_right(){
  char* title = "Settings - Sonar Right";

  #define TOP_BTN_CNT 3
  ButtonSetup topButtons[TOP_BTN_CNT] { 
    {"Low", Edit, false, sonar_right_low_edit, sonarRight.low},
    {"High", Edit, false, sonar_right_high_edit, sonarRight.high},
    {"Ping", Value, true, nullptr, sonarRight.reading}
  };
  
  #define NAV_BTN_CNT 4
  ButtonSetup navButtons[PID_NAV_CNT] { 
    {"Back", Navigation, false, fcb_edit_back}, 
    {"Enable", Toggle, sonarRight.enabled, sonar_right_enable}, 
    {"Previous", Navigation, false, sonar_next_mode}, 
    {"Next", Navigation, false, sonar_next_mode} 
  };

  // build screen for PID EDIT LEFT
  BuildScreen(title, topButtons, TOP_BTN_CNT, navButtons, NAV_BTN_CNT, FEATURE_SONAR_RIGHT); 
}

int sonar_left_low_edit(char* label)
{
  if(label[0] == '+') {
    if(sonarLeft.low+1 < sonarLeft.high){
      sonarLeft.low++;
    }    
    return sonarLeft.low;
  } else {
    if(sonarLeft.low-1 > 0){
      sonarLeft.low--;
    }    
    return sonarLeft.low;
  }  
}

int sonar_left_high_edit(char* label)
{
  if(label[0] == '+') {
    sonarLeft.high++;
    return sonarLeft.high;
  } else {
    if(sonarLeft.high-1 > sonarLeft.low){
      sonarLeft.high--;
    }    
    return sonarLeft.high;
  }  
}

int sonar_left_enable(char* label)
{
  sonarLeft.enabled = !sonarLeft.enabled;
  return sonarLeft.enabled;
}

int sonar_right_low_edit(char* label)
{
  if(label[0] == '+') {
    if(sonarRight.low+1 < sonarRight.high){
      sonarRight.low++;
    }    
    return sonarRight.low;
  } else {
    if(sonarRight.low-1 > 0){
      sonarRight.low--;
    }    
    return sonarRight.low;
  }  
}

int sonar_right_high_edit(char* label)
{
  if(label[0] == '+') {
    sonarRight.high++;
    return sonarRight.high;
  } else {
    if(sonarRight.high-1 > sonarRight.low){
      sonarRight.high--;
    }    
    return sonarRight.high;
  }   
}

int sonar_right_enable(char* label)
{
  sonarRight.enabled = !sonarRight.enabled;
  return sonarRight.enabled;
}

int sonar_next_mode(char* label)
{
  return nextMode(FEATURE_SONAR_BASE, CNT_SONAR_MODES);
}

void parent_runscreen()
{
    // Print Title
    touchDisplay.fillRect(15, 10, 300, 15, BACKGROUND_COLOR);
    touchDisplay.setCursor(15,10);
    touchDisplay.setTextColor(BLACK);
    touchDisplay.setTextSize (2);
    touchDisplay.println("Run Screen");
  
    touchDisplay.ClearTopButtons();
    touchDisplay.ClearBottomButtons();

    while (mode == PARENT_MODE_RUNSCREEN)
    {
      TSPoint p;
  
      delay(50); //debounce
      p = touchDisplay.waitOneTouch(); // Wait touch
  
      if (p.y < (TS_MINY-5)) {
        nextMode(PARENTS_BASE, CNT_PARENT_MODES);
      }
      
      touchDisplay.CheckForButtonPress(p);
    }
}

void parant_arbitrate()
{
  touchDisplay.fillRect(15, 10, 300, 15, BACKGROUND_COLOR);
  touchDisplay.setCursor(15,10);
  touchDisplay.setTextColor(BLACK);
  touchDisplay.setTextSize (2);
  touchDisplay.println("Arbitrate Screen");

  while (mode == PARENT_MODE_ARBITRATE)
  {
    TSPoint p;
    
    delay(250); //debounce
    p = touchDisplay.waitOneTouch(); // Wait touch
    
      if (p.y < (TS_MINY-5)) {
        nextMode(PARENTS_BASE, CNT_PARENT_MODES);
      }
  }  
}

int leftKedit(char* label){
  touchDisplay.UpdateStatus(label,2, Line1);
  if(label[0] == '+') {
    pids[0].K++;
    return pids[0].K;
  } else {
    pids[0].K--;
    return pids[0].K;
  }
}

int leftPedit(char* label){
  touchDisplay.UpdateStatus(label,2, Line1);
  if(label[0] == '+') {
    pids[0].P++;
    return pids[0].P;
  } else {
    pids[0].P--;
    return pids[0].P;
  }
}

int leftIedit(char* label){
  touchDisplay.UpdateStatus(label,2, Line1);
  if(label[0] == '+') {
    pids[0].I++;
    return pids[0].I;
  } else {
    pids[0].I--;
    return pids[0].I;
  }
}

int leftDedit(char* label){
  touchDisplay.UpdateStatus(label,2, Line1);
  if(label[0] == '+') {
    pids[0].D++;
    return pids[0].D;
  } else {
    pids[0].D--;
    return pids[0].D;
  }
}

int rightKedit(char* label){
  touchDisplay.UpdateStatus(label,2, Line1);
  if(label[0] == '+') {
    pids[1].K++;
    return pids[1].K;
  } else {
    pids[1].K--;
    return pids[1].K;
  }
}

int rightPedit(char* label){
  touchDisplay.UpdateStatus(label,2, Line1);
  if(label[0] == '+') {
    pids[1].P++;
    return pids[1].P;
  } else {
    pids[1].P--;
    return pids[1].P;
  }
}

int rightIedit(char* label){
  touchDisplay.UpdateStatus(label,2, Line1);
  if(label[0] == '+') {
    pids[1].I++;
    return pids[1].I;
  } else {
    pids[1].I--;
    return pids[1].I;
  }
}

int rightDedit(char* label){
  touchDisplay.UpdateStatus(label,2, Line1);
  if(label[0] == '+') {
    pids[1].D++;
    return pids[1].D;
  } else {
    pids[1].D--;
    return pids[1].D;
  }
}

int LeftPidToggleEnable(char* label)
{  
  fcbPidLeft = !fcbPidLeft;
  delay(200);
  return fcbPidLeft;
}

int RightPidToggleEnable(char* label)
{
  fcbPidRight = !fcbPidRight;
  delay(200);
  return fcbPidRight;
}

int nextPidMode(char* label)
{  
  return nextMode(FEATURE_PID_BASE, CNT_PID_MODES);
}



