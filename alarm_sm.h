/*
 * alarm_sm.h
 *
 * Created: 6/12/2019 5:50:53 PM
 *  Author: nguyenhangthien
 */ 


#ifndef ALARM_SM_H_
#define ALARM_SM_H_

#include <stdlib.h>
#include "globals_common.h"
#include "clock.h"

#define ALARM_PORT PORTA
#define ALARM_GATE 0
#define ALARMSM_PERIOD 250
#define ALARM_BLINK_FLAG_CTR 500/ALARMSM_PERIOD

typedef struct{
	int hour, minute; // will triggered when hour and minute matches clock
} Alarm;

enum alarm_states {init_alarm, check_alarm, edit_alarm, play_sound};
/* We implement an alarm tick function that 
 *	0. Checks to see if alarm is even set
 *  1. Checks if time matches with the set alarm time*/

// if alarm flag != 0 means alarm is set
extern Clock * clk;		// defined in main
extern Alarm * alarm;	// defined in main
extern enum screens focus;  // to check if we are at alarm's screen
extern int key_buf;     // to get the key_pressed buffer 
extern enum alarm_edit alarm_curr_edit;
extern unsigned char alarm_flag;
extern unsigned char alarm_blink_flag_ctr;
extern unsigned char blink_flag;

Alarm * alarm_init(int hour, int minute);
int tick_alarm(int curr_state);

char* alarm_time(Alarm * alrm, int alarm_curr_edit);
void display_alarm(Alarm *, int alarm_curr_edit);

void flip_sound();
void sound_off();

void increment_alarm(int what, Alarm *);
void decrement_alarm(int what, Alarm*);




#endif /* ALARM_SM_H_ */