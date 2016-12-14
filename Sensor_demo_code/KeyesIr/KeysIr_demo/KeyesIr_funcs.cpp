/******************************************************************
*
*  keyesir_funcs.cpp
*  version: 20161130             Steve Edwards
*  
*  Bumper functions for DPRG Club Robot 2016.
******************************************************************/

#include "keyesir_funcs.h"

// global variables
const int LEFT_IR_ENABLE_PIN = 50;
const int RIGHT_IR_ENABLE_PIN = 52;
const int LEFT_IR_PIN = 53;
const int RIGHT_IR_PIN = 51;

bool objectDetect;

// functions
// func prototypes
void init_keyes_ir()
{
  // initialize the IR output pin as a input:
  pinMode(LEFT_IR_PIN, INPUT);
  pinMode(RIGHT_IR_PIN, INPUT);

  // initialize the IR Enable as an output:
  pinMode(LEFT_IR_ENABLE_PIN, OUTPUT);
  pinMode(RIGHT_IR_ENABLE_PIN, OUTPUT);
}

unsigned char read_keyesIr()
{
  unsigned char results = 0;

  // LEFT IR
  //
  digitalWrite(LEFT_IR_ENABLE_PIN, HIGH); // Enable the internal 38kHz signal.

  delayMicroseconds(210);       // Wait 210µs (8 pulses of 38kHz).
  if (digitalRead(LEFT_IR_PIN)) // If detector Output is HIGH,
  {
    objectDetect = false; // then no object was detected;
  }
  else // but if the Output is LOW,
  {
    delayMicroseconds(395);       // wait for another 15 pulses.
    if (digitalRead(LEFT_IR_PIN)) // If the Output is now HIGH,
    {                             // then first Read was noise
      objectDetect = false; // and no object was detected;
    }
    else // but if the Output is still LOW,
    {
      results |= (0x02);
      objectDetect = true; // then an object was truly detected.
    }
  }

  digitalWrite(LEFT_IR_ENABLE_PIN, LOW); // Enable the internal 38kHz signal.

  // RIGHT IR
  //
  digitalWrite(RIGHT_IR_ENABLE_PIN, HIGH); // Enable the internal 38kHz signal.

  delayMicroseconds(210);       // Wait 210µs (8 pulses of 38kHz).
  if (digitalRead(RIGHT_IR_PIN)) // If detector Output is HIGH,
  {
    objectDetect = false; // then no object was detected;
  }
  else // but if the Output is LOW,
  {
    delayMicroseconds(395);       // wait for another 15 pulses.
    if (digitalRead(RIGHT_IR_PIN)) // If the Output is now HIGH,
    {                             // then first Read was noise
      objectDetect = false; // and no object was detected;
    }
    else // but if the Output is still LOW,
    {
      results |= (0x01);
      objectDetect = true; // then an object was truly detected.
    }
  }

  digitalWrite(RIGHT_IR_ENABLE_PIN, LOW); // Enable the internal 38kHz signal.

  return (results);
}
