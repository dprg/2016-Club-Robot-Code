/******************************************************************
*
*  ultrasonic_funcs.h
*  version: 20161011-0           Doug Paradis
*  Ultrasonic sensor functions for DPRG Club Robot 2016.
*
******************************************************************/

#ifndef USONIC_FUNCS_H
#define USONIC_FUNCS_H

typedef struct us_sens_t {
	uint16_t trigger_pin_arduino;
	uint16_t echo_pin_arduino;
	uint8_t trigger_pin;
	uint8_t echo_pin;
	uint8_t triggered_flg;
	uint8_t ready_flg;
	uint32_t start_time;
	uint32_t end_time;
	float dist;
}us_sensor;

//externs
extern us_sensor *ptr_us_R;
extern us_sensor *ptr_us_L;
extern volatile uint8_t usonic_flg;
extern volatile uint32_t start_time, end_time;

// function prototypes
void init_ultrasonic (us_sensor *ptr_usensorR, us_sensor *ptr_usensorL);
void send_us_trig(us_sensor *ptr_us_sensor);
void measure_us_dist(us_sensor *ptr_us_sensor);


#endif