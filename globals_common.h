/*
 * globals_common.h
 *
 * Created: 6/13/2019 3:12:23 PM
 *  Author: nguyenhangthien
 */ 


#ifndef GLOBALS_COMMON_H_
#define GLOBALS_COMMON_H_

#include "avr.h"
#include "lcd.h"

extern unsigned char blink_flag;

enum button {up = 2, down = 10, left = 5, right = 7, 
	edit = 6, tog_mil = 13, tog_alarm = 15, mode = 4};
enum testblink_states {tbnone=0, on, off};
enum time_type {none, hour, minute, second, mil, day, month, year};
enum screens {_init, _clock, _alarm};
enum alarm_edit{alrm_hr, alrm_min};
	
void toggle_blink_flag();


#endif /* GLOBALS_COMMON_H_ */