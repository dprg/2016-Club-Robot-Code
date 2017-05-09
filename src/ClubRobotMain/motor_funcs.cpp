/************************************************************
*
* motor_funcs.cpp
*  version: 20161020-0           Doug Paradis
* Motor/ Encoder/PID functions for DPRG Club Robot 2016.
*
*  Motor/Encoder functions are derived from:
*    - Teensy Encoder library by pjrc
*    - MonsterMoto Shield Example Sketch by Jim Lindblom
*
*    note: 
*      enc => 3292.4 pulse/rev
*      wheel is 100mm => c = pi*dia = 31.4159cm
*      pulse/cm = 3292.4pulse/rev / 31.4259cm/rev = 104.8pulse/cm 
*      if period of measure is 10ms (i.e., 100 periods/sec)
*         speed in cm/sec = cm / 100 period * 104.8 pulse/cm = 1.048 pulse/period
*
************************************************************/

#include "motor_funcs.h"

Encoder encRight(2, 3);  // Mega INT4 and INT5
Encoder encLeft(21, 20); // Mega INT0 and INT1

//  VNH2SP30 pin definitions
int inApin[2] = {7, 4}; // INA: Clockwise input
int inBpin[2] = {8, 9}; // INB: Counter-clockwise input
int pwmpin[2] = {5, 6}; // PWM input
int cspin[2] = {2, 3};  // CS: Current sense ANALOG input (note: CS is ~0.13 V/A)
int enpin[2] = {0, 1};  // EN: Status of switches output (Analog pin)

// PID setup
double setpoint[2] = {55, 55};
double pid_input[2];
double pid_output[2];

double kp[2] = {3, 3}; // these are not optimized
double ki[2] = {5, 5};
//double kp[2] = {3,3};   // these are not optimized
//double ki[2] = {16,16};
double kd[2] = {0, 0};

PID right_mtr_pid(&pid_input[0], &pid_output[0], &setpoint[0], kp[0], ki[0], kd[0], DIRECT);
PID left_mtr_pid(&pid_input[1], &pid_output[1], &setpoint[1], kp[1], ki[1], kd[1], DIRECT);

//---------- Functions -------------------------------
void init_pids()
{
  right_mtr_pid.SetSampleTime(10);
  right_mtr_pid.SetMode(AUTOMATIC);
  //right_mtr_pid.SetOutputLimits(20,240);   //min, max
  left_mtr_pid.SetSampleTime(10);
  left_mtr_pid.SetMode(AUTOMATIC);
  //left_mtr_pid.SetOutputLimits(20,240);   //min, max
}

void init_motor_driver_shield()
{
  // motor shield pin setup
  // Initialize digital pins as outputs
  for (int i = 0; i < 2; i++)
  {
    pinMode(inApin[i], OUTPUT);
    pinMode(inBpin[i], OUTPUT);
    pinMode(pwmpin[i], OUTPUT);
  }
  // Initialize braked
  for (int i = 0; i < 2; i++)
  {
    digitalWrite(inApin[i], LOW);
    digitalWrite(inBpin[i], LOW);
  }
}

void motorOff(int motor)
{
  // Initialize braked
  for (int i = 0; i < 2; i++)
  {
    digitalWrite(inApin[i], LOW);
    digitalWrite(inBpin[i], LOW);
  }
  analogWrite(pwmpin[motor], 0);
}

/* motorGo() will set a motor going in a specific direction
 the motor will continue going in that direction, at that speed
 until told to do otherwise.
 
 motor: this should be either 0 or 1, will selet which of the two
 motors to be controlled
 
 direct: Should be between 0 and 3, with the following result
 0: Brake to VCC
 1: Clockwise
 2: CounterClockwise
 3: Brake to GND
 
 pwm: should be a value between 0 and 255, higher the number, the faster
 it'll go
 */
void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm)
{
  if (motor <= 1)
  {
    if (direct <= 4)
    {
      // Set inA[motor]
      if (direct <= 1)
        digitalWrite(inApin[motor], HIGH);
      else
        digitalWrite(inApin[motor], LOW);

      // Set inB[motor]
      if ((direct == 0) || (direct == 2))
        digitalWrite(inBpin[motor], HIGH);
      else
        digitalWrite(inBpin[motor], LOW);

      analogWrite(pwmpin[motor], pwm);

      // note: CS is ~0.13 V/A
      //if ((analogRead(cspin[0]) < CS_THRESHOLD) && (analogRead(cspin[1]) < CS_THRESHOLD))
      //digitalWrite(statpin, HIGH);
      //else
      //digitalWrite(statpin, LOW);
    }
  }
}

/* --------------------------------------------------------------- */
/* motor command utilities - taken from work by D. Anderson        */
/* --------------------------------------------------------------- */
/* clip int value to min and max */

int clip(int val, int min, int max)
{
  if (val > max)
    return max;
  if (val < min)
    return min;
  return val;
}

/* clip float value to min and max */

int clip_f(float val, float min, float max)
{
  if (val > max)
    return max;
  if (val < min)
    return min;
  return val;
}

/* --------------------------------------------------------------- */
/* slew rate generator */
/* --------------------------------------------------------------- */

int slew(int from, int to, int rate)
{
  int x;

  if (to > from)
  {
    x = from + rate;
    if (x <= to)
      return (x);
  }
  else if (to < from)
  {
    x = from - rate;
    if (x >= to)
      return (x);
  }
  return to;
}
