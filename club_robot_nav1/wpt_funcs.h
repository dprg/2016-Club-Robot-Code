/******************************************************************
*
*  wpt_funcs.cpp
*  version: 20161011-0           Doug Paradis
*  Waypoint functions for DPRG Club Robot 2016.
*
******************************************************************/

#ifndef WPT_FUNCS_H
#define WPT_FUNCS_H

#include "nav_funcs.h"
#include "helpful_defines.h"

typedef struct temp_waypoint_t {
	int orig_waypt_num;
	float orig_x_target;
	float orig_y_target;
	uint8_t flgs;
	float temp_waypt_x;
	float temp_waypt_y;
}temp_waypoint;

//externs
extern temp_waypoint *ptr_temp_wpt;

extern float waypoint_x[16];   // allows for a 15 segment path and a end of path value
extern float waypoint_y[16];
extern int wayptnum;
extern uint8_t stop_movement_flg;
extern uint8_t slow_movement_flg;



// func prototypes -- available to other files
void init_temp_waypoint(temp_waypoint *ptr_t_wpt);
void create_temp_waypoint(temp_waypoint *ptr_t_wpt, location *ptr_location, float turn_angle, float detour_dist);
//int delta_target(target *ptr_target, float waypoint_x, float waypoint_y);
int delta_target(target *ptr_target, temp_waypoint *ptr_t_wpt, float waypoint_x, float waypoint_y,float dec_dist);
#endif