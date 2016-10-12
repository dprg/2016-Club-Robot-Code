/******************************************************************
*
*  nav_funcs.cpp
*  version: 20161011-0           Doug Paradis
*  Navigation functions for DPRG Club Robot 2016.
*  Based on work by David Anderson
*
******************************************************************/


#include "nav_funcs.h"
#include <math.h>

// func prototypes - this file only
float toRad(float dir_in_deg);
float toDeg(float dir_in_rad);



// Variables
location loc;
location *ptr_loc = &loc;

target targ;
target *ptr_targ = &targ;

long prev_enc_cnts[2];


//************************ Functions *************************************

void calc_encoder_interval_cnts (location *ptr_location)
{	
	long current_enc_cnts[2];
	current_enc_cnts[R_MTR] = encRight.read();
	current_enc_cnts[L_MTR] = -encLeft.read();
	long delta_enc_cnts[2];
	
	delta_enc_cnts[R_MTR] = current_enc_cnts[R_MTR] - prev_enc_cnts[R_MTR];
	delta_enc_cnts[L_MTR] = current_enc_cnts[L_MTR] - prev_enc_cnts[L_MTR];
	prev_enc_cnts[R_MTR] = current_enc_cnts[R_MTR];
	prev_enc_cnts[L_MTR] = current_enc_cnts[L_MTR];
	
	ptr_location->encoder_interval_cnt_R_mtr = delta_enc_cnts[R_MTR];
	ptr_location->encoder_interval_cnt_L_mtr = delta_enc_cnts[L_MTR];	
}


void odometer(location *ptr_location)
{
	calc_encoder_interval_cnts(ptr_location);

	// convert longs to floats and ticks to cm 
   float left_cm = (float)ptr_location->encoder_interval_cnt_R_mtr / R_MTR_CLICKS_PER_CM;  
   float right_cm = (float)ptr_location->encoder_interval_cnt_L_mtr / L_MTR_CLICKS_PER_CM; // Left motor going fwd is neg

	// calculate distance we have traveled since last sampling 
   float cm = (left_cm + right_cm) / 2;

   // accumulate total rotation around our center 
   ptr_location->theta += (left_cm - right_cm) / WHEEL_BASE;


   // and clip the rotation to plus or minus 360 degrees
   ptr_location->theta= ptr_location->theta - (float) (TWOPI * ((int)(ptr_location->theta/TWOPI)));

   ptr_location->deg_theta = (360 / TWOPI) * ptr_location->theta;


  // calculate and accumulate our position in cm (note: using movement in Y as going straight forward)
   ptr_location->x_pos += cm * sin(ptr_location->theta);
   ptr_location->y_pos += cm * cos(ptr_location->theta);
 


}

/*
int delta_target(target *ptr_target, temp_waypoint *ptr_t_wpt, float waypoint_x, float waypoint_y)
{
	int segment = 0;

	if((ptr_target->target_distance < 0.5) && (ptr_target->target_distance > -0.5))
	{
		segment = 1;
	}
	if ((ptr_t_wpt->flgs == 1) && (segment == 1)){         // temp waypt in play
		segment = 2;
	}


	return(segment);

}
*/

void locate_target(target *ptr_target, location *ptr_location) 
{
            float x,y;
        	x = ptr_target->x_target - ptr_location->x_pos;
        	y = ptr_target->y_target - ptr_location->y_pos;


        	ptr_target->target_distance = sqrt((x*x)+(y*y));
	
        	/* no divide-by-zero allowed! */
			//  1.570796 (ie, HALFPI) equals 90 deg in rads

     		if (x > 0.00001) ptr_target->target_bearing = HALFPI - atan(y/x);
        	else if (x < -0.00001) ptr_target->target_bearing = -HALFPI - atan(y/x);


        	ptr_target->heading_error = ptr_target->target_bearing - ptr_location->theta;
        	if (ptr_target->heading_error > PI) 
				ptr_target->heading_error -= TWOPI;
	        else if (ptr_target->heading_error < -PI) 
				ptr_target->heading_error += TWOPI;


			ptr_target->deg_heading_error = (360.0/TWOPI) * ptr_target->heading_error;
			

}



float toRad(float dir_in_deg){
	float dir_in_rad;
	dir_in_rad = dir_in_deg * TWOPI /360.0;
	return(dir_in_rad);
}

float toDeg(float dir_in_rad){
	float dir_in_deg;
	dir_in_deg = (360.0 / TWOPI) * dir_in_rad;
	return(dir_in_deg);
}


int determine_rotate_in_place_flg( target * ptr_target )
{
	int rotate_in_place_flg = OFF;

	if (fabs(ptr_target->deg_heading_error) >= 5.0)  // deg 
	{
		rotate_in_place_flg = ON;
	}

	if (fabs(ptr_target->deg_heading_error) <= 0.50)   // deg
	rotate_in_place_flg = OFF;
	
	return (rotate_in_place_flg);

}

