/******************************************************************
*
*  ultrasonic_funcs.h
*
*  author: Doug Paradis, 2017-18
*
*  Ultrasonic sensor functions for DPRG Club Robot 2016.
*
******************************************************************/

#ifndef USONIC_FUNCS_H
#define USONIC_FUNCS_H

typedef struct us_sens_t {
	uint8_t enable;
	uint16_t trigger_pin_arduino;
	uint16_t echo_pin_arduino;
	uint8_t trigger_pin;
	uint8_t echo_pin;
	uint8_t triggered_flg;
	uint8_t new_read_flg;
	uint32_t start_time;
	uint32_t end_time;
	uint8_t unit_num;
	uint16_t dist;
}us_sensor;

//externs
//extern us_sensor *ptr_us_R;    // uncomment when using 2 sensors 
//extern us_sensor *ptr_us_L;    // uncomment when using 2 sensors 
extern us_sensor *ptr_us;        // using only 1 sensor (comment if using 2 sensors) 
extern volatile uint8_t usonic_flg;
extern volatile uint32_t us_start_time, us_end_time;

// function prototypes


void send_us_trig(us_sensor *ptr_us_sensor);
void measure_us_dist(us_sensor *ptr_us_sensor);
void init_ultrasonic (us_sensor *ptr_usensor);             // using only one sensor (comment if using 2 sensors) 
void eval_ultrasonic_sensor(us_sensor *ptr_ultrasonic);    // using only one sensor (comment if using 2 sensors) 
//void init_ultrasonic (us_sensor *ptr_usensorR, us_sensor *ptr_usensorL);  // uncomment if 2 sensors
//void eval_ultrasonic_sensors(us_sensor *ptr_ultrasonic_R, us_sensor *ptr_ultrasonic_L); // uncomment if 2 sensors




#endif