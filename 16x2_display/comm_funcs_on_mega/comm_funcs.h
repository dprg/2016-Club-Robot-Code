/******************************************************************
*
*  comm_funcs.h
*  version: 20170720-0           Doug Paradis
*  Communication functions for DPRG Club Robot 2016.
*  Based on ideas from Ron Grant
*
******************************************************************/
#ifndef COMM_FUNCS_H
#define COMM_FUNCS_H

// Includes
#include <stdint.h>


// defines

// typedefs
// This typedef enum must be the same as the enum in the display pgm
typedef enum {
	SONAR_DATA_CMD,        //0
	SCOOP_SEN_DATA_CMD,    //1
	GATE_SERVO_DATA_CMD,   //2
	CAMERA_DATA_CMD,       //3
	ENCODER_DATA_CMD,      //4
	RUN_DATA_CMD,          //5
	NO_CMD_SENT,           //6 in this case, add additional commands before this line
	MISSING_HEADER         //7 error code for when 1 char of cmd pair is not 255
} COMM_CMDS;

// externs
extern COMM_CMDS comm_cmd;
//extern int data_val;

// Function Prototypes
uint8_t get_cmd_from_display();
void send_data_to_display(int num_of_data_val, int *data_array);
void communicate_display_data(uint8_t cmd);













#endif