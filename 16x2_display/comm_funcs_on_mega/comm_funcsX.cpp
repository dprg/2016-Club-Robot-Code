/******************************************************************
*
*  comm_funcs.cpp
*  version: 20170720-0           Doug Paradis
*  Communication functions for DPRG Club Robot 2016.
*  Based on ideas from Ron Grant
*
******************************************************************/

#include "comm_funcs.h"
#include "task_def.h"
#include "nav_funcs.h"
#include "ultrasonic_funcs.h"
#include "motor_funcs.h"
#include "scoop_sensor_funcs.h"
#include "gate_funcs.h"
#include "pixy_funcs.h"
#include "helpful_defines.h"

// global variables
uint8_t first_run_pass_flg = 0;



/*****************************************************
*
*  data packet (min size 4 bytes, add 3 bytes
*               per additional data value)
*
*      start of packet 0xFF (0xFF reserved for header)
*      for each data value
*        byte 1   7 bits of lower byte of int (least significant)
*        byte 2   8 bits of upper byte of int shifted to be the
                  8th thru 15th bits of the integer
*		 byte 3   sign byte (0xf0 = neg, else pos) 
*
*      Only the header can be 0xFF
*
*  range of values is -32639 to 32639 (-0x7F7F to 0x7F7F) 
*  no chksum 
*
******************************************************/
void send_data_to_display(int num_of_data_val,int *data_array){
	int val = 0;
	uint8_t sign = 0x00;     // positive
	uint8_t least_byte, most_byte;
	
	// send the header
	Serial3.write(0xFF);	
	
	for (int j = 0; j < num_of_data_val; j++) {
		val = data_array[j];
		if (val < 0) {
			val *= -1;      // make value pos
			sign = 0xF0;    // negative (dec = 240)
		}
		else {
			sign = 0x0F;    // positive (dec = 15)
		}
		
		least_byte = val & 0x7F;
		most_byte = val>>7;
		
		// Note: Serial3 used to communicate to UNO working with display
		Serial3.write(least_byte);       // send least significant 7 bits  range 0..127
		Serial3.write(most_byte);         // send most significant bits (bits 8 thru 15)
		Serial3.write(sign);	
	
	}
	
}


uint8_t get_cmd_from_display(){
	uint8_t val = 0;
	uint8_t sign = 0x00;     // positive
	uint8_t least_byte, most_byte;
	
	// if there's any data available from the display, read it:
	if (Serial3.available() >= 2) {
		val = Serial3.read();  
		if (val == 0xFF){           // if header, read cmd
			val = Serial3.read();   //cmd
		}
		else{
			val = MISSING_HEADER;
		}
	}
	else {
		val = NO_CMD_SENT;
	}
	return (val);                   // cmd value
}


void communicate_display_data(uint8_t cmd) {
	int num_of_data_val;
	int data_values[10] = {0,0,0,0,0,0,0,0,0,0};
	static uint8_t gate_servo_loop_cnt = 0;

	switch ((int) cmd)
	{
		case SONAR_DATA_CMD:
			num_of_data_val = 2;
			//eval_ultrasonic_sensors(ptr_us_R,ptr_us_L);    // for use when using 2 ultrasonic sensors
			eval_ultrasonic_sensor(ptr_us);                  // comment if using 2 ultrasonic sensors  
			data_values[0] = ptr_us->dist;
			data_values[1] = 0;                // only using 1 us sensor, 2nd sensor dist would go here
			if ((ptr_us->new_read_flg == ON)){    //  && (ptr_us_L->new_read_flg == ON)){
				send_data_to_display(num_of_data_val,data_values);
				ptr_us->new_read_flg = OFF;      // comment out if using 2 US sensors     
				//ptr_us_R->new_read_flg = OFF;  // uncomment if using 2 US sensors
				//ptr_us_L->new_read_flg = OFF;  // uncomment if using 2 US sensors 
			}
		break;

		case SCOOP_SEN_DATA_CMD:
			num_of_data_val = 1;
			data_values[0] = eval_scoop_sensor(ptr_scoop_sen);
			send_data_to_display(num_of_data_val,data_values);
		break;
		
		case GATE_SERVO_DATA_CMD:
			num_of_data_val = 1;
			
			SoftwareServo::refresh();    // not sure if this the best place - seems to work without
			
			gate_servo_loop_cnt++ ;
			if (gate_servo_loop_cnt >= 50){  // used with delay in display pgm to set toggle interval
				toggle_gate_state(ptr_gate);
				gate_servo_loop_cnt = 0;
			}
			data_values[0] = (int) ptr_gate->result;
			send_data_to_display(num_of_data_val,data_values);
		break;
		
		case CAMERA_DATA_CMD:
			num_of_data_val = 1;
			readCamera(ptr_pixy);
			data_values[0] = ptr_pixy->result;
			send_data_to_display(num_of_data_val,data_values);
		break;
		
		case ENCODER_DATA_CMD:
			num_of_data_val = 2;
						
			data_values[0] = (int16_t) encRight.read();
			data_values[1] = (int16_t) -encLeft.read();
			send_data_to_display(num_of_data_val,data_values);
		break;
		
		case RUN_DATA_CMD:

			if (first_run_pass_flg == 0){
				first_run_pass_flg = 1;
				
				// reset encoders and gate
				init_encoders();
				open_gate(ptr_gate);
				
				// used with real time operating system
				semaphore_release(&scoop_sem);
				semaphore_release(&us_sem);
				semaphore_release(&cam_sem);
				semaphore_release(&move_sem);


			}
			

			num_of_data_val = 3;
			data_values[0] = (int16_t) ptr_loc->x_pos;
			data_values[1] = (int16_t) ptr_loc->y_pos;
			data_values[2] = (int16_t) ptr_loc->deg_theta;
			
			send_data_to_display(num_of_data_val,data_values);
			
		
		break;
		
		default:
		break;
	}

}
