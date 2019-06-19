/*
 * clock_sm.h
 *
 * Created: 6/11/2019 9:48:28 PM
 *  Author: nguyenhangthien
 */ 


#ifndef CLOCK_SM_H_
#define CLOCK_SM_H_

#include <avr/io.h>
#include "keypad.h"
#include "avr.h"
#include "clock.h"
#include "globals_common.h"

#define CLOCKSM_PERIOD 500
#define KEEPTIME_COUNTER_CHECK 1000/CLOCKSM_PERIOD-1

extern Clock * clk;
extern unsigned char blink_flag;
enum clock_state {init, keep, set};
extern enum time_type curr_edit;
extern enum button key_pressed;
extern int keep_time_counter;
extern int key_buf;
extern enum screens focus;
extern unsigned char alarm_flag;

int tick_clk(int state); // take current state, modify appropriate var
void state_machine(Clock * clk);

/* Helper functions below */
char* get_time();			//format hh:mm:ss, hh depending on char military
char* get_date();			//format mm/dd/yy
void run_clock(Clock* clk);
void display_time(Clock* clk, int edit_f);
void cycle_right(); // cycle right edit set
void cycle_left();  // same
void increment(int edit_f, Clock * clk);
void decrement(int edit_f, Clock * clk);



#endif /* CLOCK_SM_H_ */