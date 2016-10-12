/******************************************************************
*
*  wpt_funcs.cpp
*  version: 20161011-0           Doug Paradis
*  Waypoint functions for DPRG Club Robot 2016.
*
******************************************************************/


#include <math.h>
#include "wpt_funcs.h"

// func prototypes - this file only
void reset_waypt_arrays();

// Variables
uint8_t stop_movement_flg = OFF;
uint8_t slow_movement_flg = OFF;


/*-----------------------------
                     ^y
					 |         X axis is flipped
					 |
					 |
		  x<---------+	
------------------------------*/        
					

// allows for a 15 segment path and a end of path value
// first element must be 0 for both arrays (units = cm)
float waypoint_x[16] = {0.0,  0.0,200.0,200.0,0.0,LAST_ELEM};   // CCW 1 meter sq  
float waypoint_y[16] = {0.0,200.0,200.0,  0.0,0.0,LAST_ELEM};  
	
//float waypoint_x[16] = {0.0,-100.0,-100.0,  0.0,0.0,LAST_ELEM};   // CW 1 meter sq
//float waypoint_y[16] = {0.0,   0.0, 100.0,100.0,0.0,LAST_ELEM}; 
	
//float waypoint_x[16] = {0.0,  0.0,LAST_ELEM};   // 2 meter straight 
//float waypoint_y[16] = {0.0,200.0,LAST_ELEM};
	
//float waypoint_x[16] = {0.0,  0.0, 30.0,30.0,0.0,LAST_ELEM};   // CCW 1 meter sq
//float waypoint_y[16] = {0.0,100.0,100.0, 0.0,0.0,LAST_ELEM};
	
//float waypoint_x[16] = {0.0,  0.0,100.0,100.0,0.0,0,LAST_ELEM};   // CCW 1 meter sq w/ turn at end
//float waypoint_y[16] = {0.0,100.0,100.0,  0.0,0.0,100,LAST_ELEM};

//float waypoint_x[16] = {0.0,100.0,100.0,  0.0,0.0,100.0,LAST_ELEM};   // CW 1 meter sq sq w/ turn at end
//float waypoint_y[16] = {0.0,  0.0,100.0,100.0,0.0,  0.0,LAST_ELEM};
	

//variables	
temp_waypoint temp_wpt;
temp_waypoint *ptr_temp_wpt = &temp_wpt;
	
int wayptnum = 0;




 
// ************************** Functions ***************************************
void init_temp_waypoint(temp_waypoint *ptr_t_wpt)
{
	ptr_t_wpt->orig_waypt_num = 0;
	ptr_t_wpt->orig_x_target = 0;
	ptr_t_wpt->orig_y_target = 0;
	ptr_t_wpt->temp_waypt_x = 0;
	ptr_t_wpt->temp_waypt_y = 0;
	ptr_t_wpt->flgs = 0x00;    // bit 0 = temp wpt set
}



void reset_waypt_arrays(void)
{

	// load waypoint arrays with zeros
	int elem_cnt = ARR_LENGTH(waypoint_x);
	int j = 0;

	while ( j < elem_cnt)
	{
		waypoint_x[j] = 0;
		j++;
	}
	
	j = 0;
	while ( j < elem_cnt)
	{
		waypoint_y[j] = 0;
		j++;
	}

	wayptnum = 0;
	
}

// note: turn angle is the angle you want to deviate from the current path (angle in radians)
//       detour_dist is the distance that you want to go  (dist in cm)
void create_temp_waypoint(temp_waypoint *ptr_t_wpt, location *ptr_location, float turn_angle, float detour_dist)
{
	float new_heading = 0;
	
	if (ptr_t_wpt->flgs == 0){                // temp waypoint not currently in play
		ptr_t_wpt->orig_waypt_num = wayptnum;
		ptr_t_wpt->orig_x_target = waypoint_x[wayptnum];
		ptr_t_wpt->orig_y_target = waypoint_y[wayptnum];
		ptr_t_wpt->flgs = 0x01;              // set temp wpt in place flg
	}
	
	// ck to minimize rotation of robot
	new_heading = ptr_location->theta + turn_angle;
	if (new_heading > TWOPI) {
		new_heading -= TWOPI;
	}
	if (new_heading < -TWOPI) {
		new_heading += TWOPI;
	}
	
	
	// new waypt
	ptr_t_wpt->temp_waypt_x = detour_dist * sin(new_heading) + ptr_location->x_pos;
	ptr_t_wpt->temp_waypt_y = detour_dist * cos(new_heading) + ptr_location->y_pos;
	
	// set new target
	waypoint_x[wayptnum] = ptr_t_wpt->temp_waypt_x;
	waypoint_y[wayptnum] = ptr_t_wpt->temp_waypt_y;
	
}


int delta_target(target *ptr_target, temp_waypoint *ptr_t_wpt, float waypoint_x, float waypoint_y,float dec_dist)
{
	int segment = 0;

	if((ptr_target->target_distance < 0.5) && (ptr_target->target_distance > -0.5))
	{
		segment = 1;
	}
	if ((ptr_t_wpt->flgs == 1) && (segment == 1)){         // temp waypt in play
		segment = 2;
	}
	if ((ptr_target->target_distance <= dec_dist) && (ptr_target->target_distance >= -dec_dist)){
		slow_movement_flg  |= (0x02);       //set bit2 = 1
	}
	else {
		slow_movement_flg &= ~(0x02);	    //clear bit2 = 0
	}


	return(segment);

}