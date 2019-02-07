/*****************************************************
*
*  comm_funcs.h
*
*  author: Doug Paradis, 2017-18
*
*
*
******************************************************/
#ifndef _COMM_FUNCS_H
#define _COMM_FUNCS_H

// defines
#define ONE_DATA_VAL     4
#define TWO_DATA_VAL     7
#define THREE_DATA_VAL  10
#define FOUR_DATA_VAL   13
#define FIVE_DATA_VAL   16
#define SIX_DATA_VAL    19

// typedefs
// This typedef enum must match the enum in the robot program
typedef enum {
	SONAR_DATA_CMD,        //0
	SCOOP_SEN_DATA_CMD,    //1
	GATE_SERVO_DATA_CMD,   //2
	CAMERA_DATA_CMD,       //3
	ENCODER_DATA_CMD,      //4
	RUN_DATA_CMD,          //5

} COMM_CMDS;

// externs
extern COMM_CMDS comm_cmd;
extern int comm_data[10];
extern uint8_t prev_cmd_finished_flg;


// func prototypes -- available to other files
void send_cmd(COMM_CMDS cmd);
void read_data (uint8_t num_of_data_val, int data_array[]);
void initialize_comm ();






#endif