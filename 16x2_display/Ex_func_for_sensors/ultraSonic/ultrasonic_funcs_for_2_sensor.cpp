/******************************************************************
*
*  ultrasonic_funcs.cpp
*
*  author: Doug Paradis, 2017-18
*
*  Version for use with 2 ultrasonic sensors
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

us_sensor us_R;
us_sensor *ptr_us_R = &us_R;

us_sensor us_L;
us_sensor *ptr_us_L = &us_L;


/****************************************************************
  ISR for case where there is a ultrasonic sensor connected to
  PK0 and PK1
*****************************************************************/
ISR(PCINT2_vect) 
{
	if (PINK == 0b11111101 ){               // rising edge of PK0
			us_start_time = micros();
	}

	if (PINK == 0b11111110){                // rising edge of PK1
			us_start_time = micros();
	}

	if (PINK == 0b11111100){                // falling edge
			us_end_time = micros();
			if (usonic_flg == 1){   // nothing
				usonic_flg = 2;     // meas PK0 & trig PK1
			}
			if (usonic_flg == 3) {  // nothing
				usonic_flg = 4;     // meas PK1 & reset usonic_flg = 4
			}

}

/****************************************************************************
init_ultrasonic when 2 sensors
****************************************************************************/
void init_ultrasonic (us_sensor *ptr_usensorR, us_sensor *ptr_usensorL)
{
	// We are going to use Port K as pin change interrupts for the ultrasonic
	
	// Turn all pins in port K to inputs with internal pull up resistors turned on
	DDRK = 0x00;   // all pins in port K set to input
	PORTK = 0xFF;  // all input pins in port K set with activated pull up resistors
	// note: only using PK0 and PK1 with the ultrasonics
	
	//assign pins
	ptr_usensorR->trigger_pin_arduino = 34;
	ptr_usensorR->echo_pin_arduino = A9;                     // PCINT17
	ptr_usensorR->trigger_pin = PC3;
	ptr_usensorR->echo_pin = PK1;                            // PCINT17
	ptr_usensorR->unit_num = RIGHT;
	
	ptr_usensorL->trigger_pin_arduino = 36;
	ptr_usensorL->echo_pin_arduino = A8;                     // PCINT16
	ptr_usensorL->trigger_pin = PC1;
	ptr_usensorL->echo_pin = PK0;                            // PCINT16
	ptr_usensorL->unit_num = LEFT;
	
	
	// setup pins (note: using Arduino)

	pinMode(ptr_usensor_R->trigger_pin_arduino, OUTPUT);
	pinMode(ptr_ussensorL->trigger_pin_arduino, OUTPUT);
	
	delay(5000);  <------------------------------------------------<<<<<<
	
	// enable PCINT range PCINT23:16
	PCICR |= (1 << PCIE2);
	// enable pin change interrupts PCINT16 and PCINT17
	PCMSK2 = (1 << PCINT16) | (1 << PCINT17);        // right,left
	
	// turn on global interrupts
	//sei();
}
/*********************************************************************
 eval_ultrasonic_sensors() for case where there is a sensor on
 both PK0 and PK1
***********************************************************************/
void eval_ultrasonic_sensors(us_sensor *ptr_ultrasonic_R, us_sensor *ptr_ultrasonic_L)
{
		uint8_t loop_flg = 0;
		
		while (loop_flg == 0){
			ptr_us->dist
			// ultrasonic distance sensors
			switch (usonic_flg) {
				case 0:
					delay(1);  // <------------------------<<<<<<<
					send_us_trig(ptr_ultrasonic_R);
					usonic_flg = 1;
				break;

				case 1:     
				break;
				
				case 2:
					measure_us_dist(ptr_ultrasonic_R);
					ptr_ultrasonic_R->new_read_flg = ON;
					delay(1);  //<---------------------------------------<<<<<
					send_us_trig(ptr_ultrasonic_L);
					usonic_flg = 3;
				break;
				
				case 3:     
				break;
				
				case 4:
					measure_us_dist(ptr_ultrasonic_L);
					usonic_flg = 0;
					loop_flg = 1;
					ptr_ultrasonic_L->new_read_flg = ON;
				break;		
				
				default:
				break; 
			}
		}

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
