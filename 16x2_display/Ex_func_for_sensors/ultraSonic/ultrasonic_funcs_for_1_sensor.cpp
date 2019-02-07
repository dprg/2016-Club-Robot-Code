/******************************************************************
*
*  ultrasonic_funcs.cpp
*
*  author: Doug Paradis, 2017-18
*
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
volatile uint32_t us_start_time;
volatile uint32_t us_end_time; 

us_sensor us;
us_sensor *ptr_us = &us;


/****************************************************************
  ISR for case where there is a ultrasonic sensor connected to
  only PK0. 
  Note: Since the internal pull-up resistors are on for the whole
    port, port pins not enabled in the interrupt will always be 1.
*****************************************************************/
ISR(PCINT2_vect) 
{
	uint32_t time_entry = micros(); 
	if (PINK == 0b11111111){                // rising edge of PK0
			us_start_time = time_entry;
	}

	if (PINK == 0b11111110){                // falling edge
			us_end_time =time_entry;
			if (usonic_flg == 1){   // nothing
				usonic_flg = 2;     // meas PK0
			}

	}
}


/****************************************************************************
init_ultrasonic when 1 sensors
****************************************************************************/
void init_ultrasonic (us_sensor *ptr_usensor)
{
	// We are going to use Port K as pin change interrupts for the ultrasonic
	
	// Turn all pins in port K to inputs with internal pull up resistors turned on
	DDRK = 0x00;   // all pins in port K set to input
	PORTK = 0xFF;  // all input pins in port K set with activated pull up resistors
	// note: only using PK0 with the ultrasonics
	
	//assign pins
	ptr_usensor->trigger_pin_arduino = 36;
	ptr_usensor->echo_pin_arduino = A8;                     // PCINT16
	ptr_usensor->trigger_pin = PC1;
	ptr_usensor->echo_pin = PK0;                            // PCINT16
	ptr_usensor->unit_num = 1;
	
	
	// setup pins (note: using Arduino)
	pinMode(ptr_usensor->trigger_pin_arduino, OUTPUT);
	
	// other settings
	ptr_usensor->enable = ON;
	
	delay(1000);    // <---------------------------------------------------<<<<<
	
	// enable PCINT range PCINT23:16
	PCICR |= (1 << PCIE2);
	// enable pin change interrupts PCINT16
	PCMSK2 |= (1 << PCINT16);        
	
	// turn on global interrupts
	//sei();
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
	uint32_t delta_time = 0;
	uint16_t dist = 0;               //cm
	
	ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
		 ptr_us_sensor->start_time = us_start_time;
		 ptr_us_sensor->end_time = us_end_time;	
	}
	
	delta_time = ptr_us_sensor->end_time - ptr_us_sensor->start_time;
	
	// delta_time to inches
	dist = (uint16_t) delta_time /148;     // inches/microsecond 
	
	ptr_us_sensor->triggered_flg = OFF;
	ptr_us_sensor->new_read_flg = ON;  // <------------------------------<<<<<
	ptr_us_sensor->dist = dist;
	
}


void eval_ultrasonic_sensor(us_sensor *ptr_ultrasonic)
{
	uint8_t loop_flg = 0;
	
	while (loop_flg == 0){
		
		// ultrasonic distance sensors
		switch (usonic_flg) {
			case 0:
				delay(1);  // <------------------------<<<<<<<
				send_us_trig(ptr_ultrasonic);
			usonic_flg = 1;
			
			break;

			case 1: // nothing - wait state
			break;
			
			case 2:
				measure_us_dist(ptr_ultrasonic);
				//ptr_ultrasonic->new_read_flg = ON;
				usonic_flg = 0;
				loop_flg = 1;
				delay(1);  //<---------------------------------------<<<<<
			break;
	
			default:
			break;
		}
	}

}




	