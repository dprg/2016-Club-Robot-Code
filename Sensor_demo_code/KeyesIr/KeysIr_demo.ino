/*
  Keyes IR Sensor (object avoidance)

 An example of using the Keyes IR sensor to detect object from 4 cm to 40 cm

 This example shows how to filter out noise to get a reliable object detection

 Code adapted by Steve Edwards, original code is from http://irsensor.wizecode.com/ written by bud@budryerson.com. 

 The circuit:
 * GND
 * Power (+5v)
 * Output - Signal pin
 * EN = Enable pin
 */

// this constant won't change:
const int irPinOutput = 2; // the pin that the pushbutton is attached to
const int enablePin = 3;
bool objectDetect;

void setup()
{
  // initialize the button pin as a input:
  pinMode(irPinOutput, INPUT);
  // initialize the LED as an output:
  pinMode(enablePin, OUTPUT);
  // initialize serial communication:
  Serial.begin(9600);
}

void loop()
{
  digitalWrite(enablePin, HIGH); // Enable the internal 38kHz signal.

  delayMicroseconds(210);       // Wait 210µs (8 pulses of 38kHz).
  if (digitalRead(irPinOutput)) // If detector Output is HIGH,
  {
    Serial.println("off");
    objectDetect = false; // then no object was detected;
  }
  else // but if the Output is LOW,
  {
    delayMicroseconds(395);       // wait for another 15 pulses.
    if (digitalRead(irPinOutput)) // If the Output is now HIGH,
    {                             // then first Read was noise
      Serial.println("off");
      objectDetect = false; // and no object was detected;
    }
    else // but if the Output is still LOW,
    {
      Serial.println("on");
      objectDetect = true; // then an object was truly detected.
    }
  }
  digitalWrite(enablePin, LOW); // Disable the internal 38kHz signal.
}
