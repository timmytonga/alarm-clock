/*
 * keypad_sm.h
 *
 * Created: 6/12/2019 3:11:12 PM
 *  Author: nguyenhangthien
 */ 


#ifndef KEYPAD_SM_H_
#define KEYPAD_SM_H_
/* we write a tick function to set a global variable "key_pressed"
 *  to the current pressed key by a keypad
 *  the polling period is set by the scheduler i.e. however often this gets called */

#include "keypad.h"
#include "globals_common.h"

#define KEYPADSM_PERIOD 100
#define TESTBLINK_PERIOD 200
extern enum button key_pressed; // set this variable
extern int key_buf;


int tick_keypad(int current_state){
	key_pressed = get_key();
	if (key_pressed > 0 && key_buf == 0) key_buf = key_pressed;
	return 0; 
}

extern int tb_remaining_toggle;
int tick_testblink(int curr_state){
	if ( key_pressed > 0 ) tb_remaining_toggle = key_pressed;
	switch(curr_state){
		case(tbnone):
		if(tb_remaining_toggle >= 1) curr_state = on;
		break;
		
		case(on):
		curr_state = off;
		break;
		
		case(off):
		if(tb_remaining_toggle <= 0) {
			curr_state = none;
			tb_remaining_toggle = 0;
		}
		else curr_state = on;
		break;
	}
	switch(curr_state){
		case(none):
		CLR_BIT(PORTA, 0);
		break;
		case(on):
		SET_BIT(PORTA, 0);
		break;
		case(off):
		CLR_BIT(PORTA, 0);
		tb_remaining_toggle--;
		break;
	}
	return curr_state;
};

#endif /* KEYPAD_SM_H_ */