/*
 * ui_manager.c
 *
 * Created: 6/13/2019 4:03:34 PM
 *  Author: nguyenhangthien
 */ 

#include "ui_manager.h"

int tick_ui_manager(int curr_state){
	int key = 0;
	if (key_buf != 0) key = key_buf; // do not pop here
	switch(curr_state){
		case(_init):
			curr_state = focus;
			break;
		case(_clock):
			if (key == mode) {
				curr_state = _alarm;
				focus = _alarm;
			}
			break;
		case( _alarm):
			if (key == mode ){
				curr_state = _clock;
				focus = _clock;
			}
			break;
		default:
			curr_state = _init;
			break;
	}
	
	switch(curr_state){
		case(_init):
			break;
		case( _clock):
			display_time(clk, curr_edit);
			break;
		case(_alarm):
			display_alarm(alarm, alarm_curr_edit);
			break; 
		default:
			break;
	}
	
	return curr_state;
}