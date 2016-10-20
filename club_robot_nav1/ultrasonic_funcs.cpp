/******************************************************************
*
*  ultrasonic_funcs.cpp
*  version: 20161020-0           Doug Paradis
*  Ultrasonic sensor functions for DPRG Club Robot 2016.
*
******************************************************************/

#include <arduino.h>
#include "ultrasonic_funcs.h"
#include <avr/interrupt.h>
#include <util/atomic.h>
#include "helpful_defines.h"

// Variables
volatile uint8_t usonic_flg;
volatile uint8_t portk_hist;
volatile uint32_t us_start_time, us_end_time; 

us_sensor us_R;
us_sensor *ptr_us_R = &us_R;

us_sensor us_L;
us_sensor *ptr_us_L = &us_L;



ISR(PCINT2_vect) 
{
	uint8_t changed_bits;
	
	changed_bits = PINK ^ portk_hist;
	
	if (changed_bits & (1 << PK0)) {          // PCINT16 changed
		if (portk_hist < PINK){               // rising edge
			us_start_time = micros();
		}
		else {                                // falling edge
			us_end_time = micros();
			usonic_flg = 1;
		}
		portk_hist = PINK;
	}

	if (changed_bits & (1 << PK1)) {          // PCINT17 changed
		if (portk_hist < PINK){               // rising edge
			us_start_time = micros();
		}
		else {                                // falling edge
			us_end_time = micros();
			usonic_flg = 3;
		}
		portk_hist = PINK;
	}	
	
}


void init_ultrasonic (us_sensor *ptr_usensorR, us_sensor *ptr_usensorL)
{
	// enable PCINT range PCINT23:16
	PCICR |= (1 << PCIE2);
	// enable pin change interrupts PCINT16 and PCINT17  
	PCMSK2 = (1 << PCINT16) | (1 << PCINT17);        // right,left
	
	//assign pins
	ptr_usensorR->trigger_pin_arduino = 36;
	ptr_usensorR->echo_pin_arduino = A8;                     // PCINT16
	ptr_usensorR->trigger_pin = PC1;
	ptr_usensorR->echo_pin = PK0;                            // PCINT16
	
	ptr_usensorL->trigger_pin_arduino = 34;
	ptr_usensorL->echo_pin_arduino = A9;                     // PCINT17
	ptr_usensorL->trigger_pin = PC3;
	ptr_usensorL->echo_pin = PK1;                            // PCINT17
	
	// setup pins (note: using Arduino)
	pinMode(ptr_us_R->trigger_pin_arduino, OUTPUT);
	pinMode(ptr_us_R->echo_pin_arduino,INPUT);
	pinMode(ptr_us_L->trigger_pin_arduino, OUTPUT);
	pinMode(ptr_us_L->echo_pin_arduino,INPUT);
	
	
}

void send_us_trig(us_sensor *ptr_us_sensor)
{
	PORTC &= ~(1 << ptr_us_sensor->trigger_pin);   // make sure pin low
	delayMicroseconds(20);                         // let settle
	PORTC |= (1 << ptr_us_sensor->trigger_pin);    // take pin high
	delayMicroseconds(12);                         // keep high for 12uS
	PORTC &= ~(1 << ptr_us_sensor->trigger_pin);   // take pin low
	delayMicroseconds(20);                         // let settle
	ptr_us_sensor->triggered_flg = ON;
} 

void measure_us_dist(us_sensor *ptr_us_sensor)
{
	uint16_t delta_time = 0;
	float dist = 0;               //cm
	
	ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
		 ptr_us_sensor->start_time = us_start_time;
		 ptr_us_sensor->end_time = us_end_time;		
	}
	
	delta_time = ptr_us_sensor->end_time - ptr_us_sensor->start_time;
	
	// distance to object cm = ((delta_time us / 1000000 us/sec) * 34026 cm/sec )/ 2 
	dist = .01713 * delta_time;    // ~ 17 * delta_time ms
	
	ptr_us_sensor->triggered_flg = OFF;
	ptr_us_sensor->ready_flg = OFF;
	ptr_us_sensor->dist = dist;

}