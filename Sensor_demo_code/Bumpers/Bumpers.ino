#include "bumper_funcs.h"

void setup() {
  Serial.begin(57600); 
  
  init_bumper();
}

void loop() {
  unsigned char reading = read_bumpers();

  if (reading & 0x01)
  {
    Serial.println("Right Bumper");
  }
  
  if (reading & 0x02)
  {
    Serial.println("Left Bumper");
  }
}
