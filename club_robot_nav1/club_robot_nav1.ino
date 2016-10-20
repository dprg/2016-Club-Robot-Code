/*******************************************************
*  club_robot_nav1.ino
*
*  LMX multi-tasking on ARDUINO 
*  04 Sep 2015 dpa Created.  Mega2560
*
*  version: 20161011-0           Doug Paradis
*  LMX for Arduino Mega2560 version of DPRG Club Robot 2016 .
*  
*
 *******************************************************/


#include <SoftwareServo.h>
#include <PID_v1.h>
#include <stdio.h>
#include <Encoder.h>

#include <task.h>
#include "task_def.h"
#include "motor_funcs.h"
#include "ultrasonic_funcs.h"
#include "bumper_funcs.h"
#include "claw_funcs.h"
#include "wpt_funcs.h"

void printkbuf(char *s) {
   #if ((MACHINE == MACH_AVR) || (MACHINE == MACH_ARM))
   // Serial.println(s);
   PRINTF(s);
   #endif
}




// system_init
void system_init(void)
{
	#if ((MACHINE == MACH_AVR) || (MACHINE == MACH_ARM))
		/* AVR & ARM Teesy3.1  */
		init_motor_driver_shield();
		sysclock_init();
		init_ultrasonic (ptr_us_R,ptr_us_L);
		init_bumper();
		init_claw();
		init_temp_waypoint(ptr_temp_wpt);
		Serial3.begin(57600);   
	#endif
}


/* ----------------------------------------- */
/* main */

#if ((MACHINE == MACH_AVR) || (MACHINE == MACH_ARM))  /* ARM is Teensy3.1 */
/* this is for the Arduino IDE "sketch" set up */
void setup()
#else
int main()
#endif
{
    system_init();
    printv = printkbuf;

    PRINTF("Howdy Console!\n");

    #if ((MACHINE == MACH_AVR) || (MACHINE == MACH_ARM)) /* ARM is Teensy3.1 */
    delay(1500);   /* hardware delay, sysclock not running yet */
    #endif

    pid_count = 0; current = 0;

	create_task("SENSORS",sensors,20,MINSTACK * 2);  //40
	create_task("CLAW",claw,45,MINSTACK);        //45
	create_task("MOVE",move,10,MINSTACK*2);        //25 //20
    create_task("IDLE",cpu_idle,0,MINSTACK);
    //create_task("STATS",stats_task,10000,MINSTACK*4);
    //create_task("SIGNON",signon,1,MINSTACK*4);

    scheduler();
    PRINTF("Should never get here.");

    while (1);
    #if ((MACHINE != MACH_AVR) && (MACHINE != MACH_ARM))
    return 0;
    #endif
}

void loop() 
{
  /* nothing to see here, move along */
  asm("nop");
}
    
/* ----------------------------------------- */
/* EOF */


