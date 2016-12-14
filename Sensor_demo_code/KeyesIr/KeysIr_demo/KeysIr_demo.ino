/*
  Keyes IR Sensor (object avoidance)

 An example of using the Keyes IR sensor to detect object from 4 cm to 40 cm

 This example shows how to filter out noise to get a reliable object detection

 Code adapted by Steve Edwards, original code is from http://irsensor.wizecode.com/ written by bud@budryerson.com. 

 The circuits:

 Left IR
 * GND
 * Power (+5v)
 * Output - Signal pin = pin 51
 * EN = Enable pin = pin 50

 Right IR
 * GND
 * Power (+5v)
 * Output - Signal pin = pin 53
 * EN = Enable pin = pin 52
 */

#include "KeyesIr_funcs.h"
uint16_t leftHits = 0;
uint16_t rightHits = 0;

void setup()
{
  // initialize serial communication:
  Serial.begin(57600);

  init_keyes_ir();
}

void loop()
{
  unsigned char reading = read_keyesIr();
//
//  if (reading & 0x01)
//  {
//    Serial.print("Right IR - ");
//    Serial.println(++rightHits);
//  }
  
//  if (reading & 0x02)
//  {
//    Serial.print("Left IR - ");
//    Serial.println(++leftHits);
//  }
}
