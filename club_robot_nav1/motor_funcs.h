/******************************************************************
*  motor_funcs.h
*  version: 20161020-0           Doug Paradis
*
******************************************************************/
#ifndef MOTOR_FUNCS_H
#define  MOTOR_FUNCS_H

#include <arduino.h>
#include <Encoder.h>
#include <PID_v1.h>


// Motor shield setup
#define BRAKEVCC 0
#define CW   1
#define CCW  2
#define BRAKEGND 3
 

//externs
extern int inApin[2];  // INA: Clockwise input
extern int inBpin[2];  // INB: Counter-clockwise input
extern int pwmpin[2];  // PWM input
extern int cspin[2];   // CS: Current sense ANALOG input
extern int enpin[2];   // EN: Status of switches output (Analog pin
//extern int statpin;    // status LED pin 13 - not useful, covered by shield

extern Encoder encRight;   // Mega INT4 and INT5
extern Encoder encLeft;   // Mega INT0 and INT1

extern double r_setpoint,r_pid_input,r_pid_output;
extern double l_setpoint,l_pid_input,l_pid_output;
extern double setpoint[];
extern double pid_input[];
extern double pid_output[];


extern double kp[];
extern double ki[];
extern double kd[];


extern PID right_mtr_pid;
extern PID left_mtr_pid;




// function prototypes
void init_pids ();
void init_motor_driver_shield();
void motorOff(int motor);
void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm);
int clip(int val,int min,int max);
int clip_f(float val,float min,float max);
int slew(int from, int to, int rate);




#endif