/********************************************************

 System Checks menu state machine
  
  	SONAR_TEST,               // display ultrasonic sensor output
	SCOOP_TEST,               // display scoop sensor state
	GATE_SERVO_TEST           // test and display gate servo position
  	CAMERA_TEST,             // display target location in camera field
  	ENCODERS_TEST,	         // display encoder values for the 3 encoders (check by rotating wheels or pushing robot on surface)
  	EXIT_SYS_CHKS            // return to main menu


  author: Doug Paradis, 2017-18


********************************************************/

#include "common.h"
#include "sys_chks_menu_state_machine.h"
#include "comm_funcs.h"


SYS_CHKS_STATES sys_chks_state;


SYS_CHKS_STATES sys_chks_menu_state_machine(EVENTS event)
{

	/* Switch the sys_chks_state and the event to execute the right transition. */
	switch(sys_chks_state)
	{
		case SONAR_TEST:
		switch(event)
		{
			case LEFT_BUTTON:
			sys_chks_state = SCOOP_SEN_TEST;
			break;
			case RIGHT_BUTTON:
			sys_chks_state = EXIT_SYS_CHKS;
			break;
			case SELECT_BUTTON:
			sys_chks_state = SONAR_TEST;
			run_sonar_test();
			testing_flg = OFF;
			break;
			case UP_BUTTON:
			sys_chks_state = SONAR_TEST;
			break;
			case DOWN_BUTTON:
			sys_chks_state = SONAR_TEST;
			break;
			default:
			break;
		}
		break;
		
		case SCOOP_SEN_TEST:
		switch(event)
		{
			case LEFT_BUTTON:
				sys_chks_state = GATE_SERVO_TEST;
			break;
			case RIGHT_BUTTON:
				sys_chks_state =SONAR_TEST;
			break;
			case SELECT_BUTTON:
				sys_chks_state = SCOOP_SEN_TEST;
				run_scoop_sen_test();
				testing_flg = OFF;
			break;
			case UP_BUTTON:
				sys_chks_state = SCOOP_SEN_TEST;
			break;
			case DOWN_BUTTON:
				sys_chks_state = SCOOP_SEN_TEST;
			break;
			default:
			break;
		}
		break;
		
		case GATE_SERVO_TEST:
		switch(event)
		{
			case LEFT_BUTTON:
			sys_chks_state = CAMERA_TEST;
			break;
			case RIGHT_BUTTON:
			sys_chks_state = SCOOP_SEN_TEST;
			break;
			case SELECT_BUTTON:
			sys_chks_state = GATE_SERVO_TEST;
			run_gate_servo_test();
			testing_flg = OFF;
			break;
			case UP_BUTTON:
			sys_chks_state = GATE_SERVO_TEST;
			break;
			case DOWN_BUTTON:
			sys_chks_state = GATE_SERVO_TEST;
			break;
			default:
			break;
		}
		break;
		
		case CAMERA_TEST:
		switch(event)
		{
			case LEFT_BUTTON:
				sys_chks_state = ENCODERS_TEST;
			break;
			case RIGHT_BUTTON:
				sys_chks_state = GATE_SERVO_TEST;
			break;
			case SELECT_BUTTON:
				sys_chks_state = CAMERA_TEST;
				run_camera_test();
				testing_flg = OFF;
			break;
			case UP_BUTTON:
				sys_chks_state = CAMERA_TEST;
			break;
			case DOWN_BUTTON:
				sys_chks_state = CAMERA_TEST;
			break;
			default:
			break;
		}
		break;
		
		case ENCODERS_TEST:
		switch(event)
		{
			case LEFT_BUTTON:
				sys_chks_state = EXIT_SYS_CHKS;
			break;
			case RIGHT_BUTTON:
				sys_chks_state = CAMERA_TEST;
			break;
			case SELECT_BUTTON:
				sys_chks_state = ENCODERS_TEST;
				run_encoders_test();
				testing_flg = OFF;
			break;
			case UP_BUTTON:
				sys_chks_state = ENCODERS_TEST;
			break;
			case DOWN_BUTTON:
				sys_chks_state = ENCODERS_TEST;
			break;
			default:
			break;
		}
		break;
		
		case EXIT_SYS_CHKS:
		switch(event)
		{
			case LEFT_BUTTON:
				sys_chks_state = SONAR_TEST;
			break;
			case RIGHT_BUTTON:
				sys_chks_state = ENCODERS_TEST;
			break;
			case SELECT_BUTTON:
				sys_chks_state = EXIT_SYS_CHKS;
				return_to_menu_fr_sys_chks();
			break;
			case UP_BUTTON:
				sys_chks_state = EXIT_SYS_CHKS;
			break;
			case DOWN_BUTTON:
			sys_chks_state = EXIT_SYS_CHKS;
			break;
			default:
			break;
		}
		break;
		
	}
	
	// titles on row 0 of display
	switch (sys_chks_state)
	{
		case SONAR_TEST:
			lcd.clear();
			lcd.setCursor(0,0);
			lcd.print("Sonar Test");
			lcd.setCursor(0,1);
			lcd.print("                ");   // clear line
		break;
				
		case SCOOP_SEN_TEST:
			lcd.clear();
			lcd.setCursor(0,0);
			lcd.print("Scoop Sen Test");
			lcd.setCursor(0,1);
			lcd.print("                ");   // clear line
		break;
		
		case GATE_SERVO_TEST:
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Gate Servo Test");
		lcd.setCursor(0,1);
		lcd.print("                ");   // clear line
		break;
		
		case CAMERA_TEST:
			lcd.clear();
			lcd.setCursor(0,0);
			lcd.print("Camera Test");
			lcd.setCursor(0,1);
			lcd.print("                ");   // clear line
		break;
		
		case ENCODERS_TEST:
			lcd.setCursor(0,0);
			lcd.print("Encoder Test    ");
			lcd.setCursor(0,1);
			lcd.print("                ");   // clear line
		break;
		
		case EXIT_SYS_CHKS:
			lcd.clear();
			lcd.setCursor(0,0);
			lcd.print("Back to Menu    ");
			lcd.setCursor(0,1);
			lcd.print("                ");   // clear line
		break;
		
		default:
		break;
	}
	
	return(sys_chks_state);
}

//-------------------------- additional funcs ---------------------------------------

void run_sonar_test()
{
	while (testing_flg == OFF){
		clear_LCD_line(BOTTOM);
		
		// send test cmd
		if (prev_cmd_finished_flg == DONE){
			send_cmd(SONAR_DATA_CMD);
		}
		
		// wait to data return
		//while (Serial.available() < TWO_DATA_VAL); // wait for two data values
		while (Serial.available() == 0);
		//{
		//	lcd.setCursor(15,1);
		//	lcd.print("*");
		//	delay(1);	
		//}
		
		lcd.setCursor(15,1);
		lcd.print("&");
		
		// process data
		read_data (2, comm_data);
		
		// report  results
		lcd.setCursor(0,1);               // column 0, row 1
		lcd.print("L:");
		lcd.setCursor(3,1);
		lcd.print(comm_data[0]);
		lcd.setCursor(8,1);
		lcd.print("R:");
		lcd.setCursor(11,1);
		lcd.print(comm_data[1]);
		
		delay(200);   //<-----------------------------<<<<<<
		
		stop_running_test();
	}
}

void run_scoop_sen_test()
{
	while (testing_flg == OFF){
		clear_LCD_line(BOTTOM);
		
		// send test cmd
		if (prev_cmd_finished_flg == DONE){
			send_cmd(SCOOP_SEN_DATA_CMD);
		}
		
		// wait to data return
		//while (Serial.available() < ONE_DATA_VAL); // wait for one data value
		while (Serial.available() == 0);
		
		lcd.setCursor(15,1);
		lcd.print("&");
		
		// process data
		read_data (1, comm_data);
		
		// report  results
		lcd.setCursor(0,1);               // column 0, row 1
		if (comm_data[0] == 0){
			lcd.print("Not Blocked");   
		}
		else{
			lcd.print("Blocked");       
		}
		
		stop_running_test();
	}
	
}

void run_gate_servo_test()
{
	while (testing_flg == OFF){
		clear_LCD_line(BOTTOM);
	
		// send test cmd
		if (prev_cmd_finished_flg == DONE){
			send_cmd(GATE_SERVO_DATA_CMD);
		}
	
		// wait to data return
		//while (Serial.available() < ONE_DATA_VAL); // wait for one data value
		while (Serial.available() == 0);
		
		lcd.setCursor(15,1);
		lcd.print("*");
	
		// process data
		read_data (1, comm_data);
	
		// report  results
		lcd.setCursor(0,1);               // column 0, row 1
		//lcd.print("State:");
		//lcd.setCursor(8,1);
		if (comm_data[0] == 0){
			lcd.print("Gate Raised");
		}
		else{
			lcd.print("Gate Down");
		}
		
		//delay(20);   //<-----------------------------<<<<<<
	
		stop_running_test();
	}

}

void run_camera_test()
{
	while (testing_flg == OFF){
		clear_LCD_line(BOTTOM);
		
		// send test cmd
		send_cmd(CAMERA_DATA_CMD);

		// wait to data return
		//while (Serial.available() < ONE_DATA_VAL); // wait for one data value
		while (Serial.available() == 0);
		
		lcd.setCursor(15,1);
		lcd.print("&");
		
		//while (Serial.available()) {
		
		// process data
		read_data (1, comm_data);
		//}
		
		// report  results
		lcd.setCursor(0,1);               // column 0, row 1
		lcd.print("Tgt:");
		lcd.setCursor(8,1);
		if (comm_data[0] == '#'){
			lcd.print("none");
		}
		else {
			lcd.print(comm_data[0]);
		}
		
		stop_running_test();
		
	}
}

void run_encoders_test()
{
	// plan is to have robot rotate 90 degs and return to facing forward after short delay
	while (testing_flg == OFF){
		clear_LCD_line(BOTTOM);
		
		// send test cmd
		if (prev_cmd_finished_flg == DONE){
			send_cmd(ENCODER_DATA_CMD);
		}
		
		// wait to data return
		//while (Serial.available() < TWO_DATA_VAL); // wait for two data values
		while (Serial.available() == 0);
		//while (Serial.available() == 0){
		//	lcd.setCursor(15,1);
		//	lcd.print("*");
		//	delay(1);
			
		//}
		
		// process data
		read_data (2, comm_data);
		
		// report  results
		lcd.setCursor(0,1);               // column 0, row 1
		lcd.print("L:");
		lcd.setCursor(3,1);
		lcd.print(comm_data[0]);
		lcd.setCursor(8,1);
		lcd.print("R:");
		lcd.setCursor(11,1);
		lcd.print(comm_data[1]);
		
		stop_running_test();
	}
}



void return_to_menu_fr_sys_chks()
{
	index = MAIN_MENU_INDEX;
}



