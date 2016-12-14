 /*
 * Hardware Connections:
 *
 * Left HC-SR04           
 *  Arduino | HC-SR04     
 *  -------------------   
 *    5V    |   VCC       
 *    47    |   Trig     
 *    46    |   Echo     
 *    GND   |   GND
 *
 * Right HC-SR04
 *  Arduino | HC-SR04 
 *  -------------------
 *    5V    |   VCC     
 *    49    |   Trig     
 *    48    |   Echo     
 *    GND   |   GND
 */

#define left_echoPin 46
#define right_echoPin 48
#define left_trigPin 47
#define right_trigPin 49

void setup() {
  Serial.begin (57600);
  pinMode(left_trigPin, OUTPUT);
  pinMode(left_echoPin, INPUT);
  pinMode(right_trigPin, OUTPUT);
  pinMode(right_echoPin, INPUT);
}

void loop() {
  Serial.print("Sonar: ");
  Serial.print(MeasureDistance(left_trigPin, left_echoPin));
  // Wait at least 60ms before next measurement
  delay(60);

  Serial.print(", ");

  Serial.println(MeasureDistance(right_trigPin, right_echoPin));  
  // Wait at least 60ms before next measurement
  delay(60);
}

float MeasureDistance(uint8_t trigPin, uint8_t echoPin) {

  unsigned long t1;
  unsigned long t2;
  unsigned long pulse_width;
  float cm;
  float inches;

  // Hold the trigger pin high for at least 10 us
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Wait for pulse on echo pin
  while ( digitalRead(echoPin) == 0 );

  // Measure how long the echo pin was held high (pulse width)
  // Note: the micros() counter will overflow after ~70 min
  t1 = micros();
  while ( digitalRead(echoPin) == 1);
  t2 = micros();
  pulse_width = t2 - t1;

  // Calculate distance in centimeters and inches. The constants
  // are found in the datasheet, and calculated from the assumed speed 
  //of sound in air at sea level (~340 m/s).
  cm = pulse_width / 58.0;
  inches = pulse_width / 148.0;

  return cm;
}
