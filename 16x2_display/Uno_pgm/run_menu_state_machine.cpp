/********************************************************

  Run menu run_state machine


  author: Doug Paradis, 2017-18


********************************************************/

#include "common.h"
#include "run_menu_state_machine.h"
#include "comm_funcs.h"

RUN_STATES run_state;

RUN_STATES run_menu_state_machine(EVENTS event)
{

	/* Switch the run_state and the event to execute the right transition. */
	switch(run_state)
	{
		case WAITING:
		switch(event)
		{
			case SELECT_BUTTON:
			run_state = RELEASED;
			start_robot();
			delay(500);
			clear_LCD();
			update_run_data();
			default:
			break;
		}
		break;
		
		case RELEASED:
		switch(event)
		{
			case SELECT_BUTTON:
			run_state = RELEASED;
			//update_run_data();
			break;
			default:
			break;
		}
		break;
		

		
	}
	
	switch (run_state)
	{
		case WAITING:
		lcd.setCursor(0,0);
		lcd.print("Waiting         ");
		break;
		
		case RELEASED:
		lcd.setCursor(0,0);
		//lcd.print("Released        ");
		
		break;
		default:
		break;
	}
	
	return(run_state);
}

//-------------------------------additional funcs----------------------------
void start_robot()
{
	//lcd.clear();
	//lcd.setCursor(0,0);               // column 0, row 1
	//lcd.print("Start Robot     ");
	for(int j = 3;j > 0; j--){
		lcd.setCursor(7,1);
		lcd.print(j);
		delay(1000);		
	}
	// send go cmd
	lcd.setCursor(0,0);
	clear_LCD_line(0);
	lcd.setCursor(3,1);
	lcd.print("Engaged");
	delay(1000);	
	update_run_data();
	
	
}

void update_run_data()
{
	while (testing_flg == OFF){
		
		//clear_LCD();
		
		// send test cmd
		send_cmd(RUN_DATA_CMD);
		
		// non-data parts of run LCD display
		//x
		lcd.setCursor(0,0);               // column 0, row 0
		lcd.print("x: ");                 // x_pos
		lcd.setCursor(3,0);
		lcd.print("     ");
		//y
		lcd.setCursor(3,0);
		lcd.print(comm_data[0]);
		lcd.setCursor(8,0);
		lcd.print("y: ");                 // y_pos
		lcd.setCursor(11,0);
		lcd.print("      ");
		//theta
		lcd.setCursor(0,1);               // column 0, row 1
		lcd.print("theta: ");                 // theta_deg
		lcd.setCursor(7,1);
		lcd.print("         ");
		
		
		// wait to data return
		//while (Serial.available() < THREE_DATA_VAL); // wait for one data value
		while (Serial.available() == 0);
		
		lcd.setCursor(15,1);
		lcd.print("&");
		
		// process data
		read_data (3, comm_data);
		
		// report  results
		//lcd.setCursor(0,0);               // column 0, row 0
		//lcd.print("x: ");                 // x_pos
		//lcd.setCursor(3,0);
		//lcd.print("    ");
		// x
		lcd.setCursor(3,0);
		lcd.print(comm_data[0]);
		//lcd.setCursor(8,0);               
		//lcd.print("y: ");                 // y_pos
		//lcd.setCursor(11,0);
		//lcd.print("    ");
		lcd.setCursor(11,0);
		lcd.print(comm_data[1]);
		//lcd.setCursor(0,1);               // column 0, row 1
		//lcd.print("T: ");                 // theta_deg
		//lcd.setCursor(3,1);
		//lcd.print("    ");
		lcd.setCursor(7,1);
		lcd.print(comm_data[2]);
		
		// add delay to data request
		delay(100);    // only ask for data every ~100ms
				
		stop_running_test();
	}
	
}



