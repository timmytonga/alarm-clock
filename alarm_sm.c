/*
 * alarm_sm.c
 *
 * Created: 6/13/2019 2:22:36 PM
 *  Author: nguyenhangthien
 */

#include "alarm_sm.h"

Alarm * alarm_init(int hr, int min){
	Alarm * newalrm = malloc(sizeof(Alarm));
	newalrm->hour = hr;
	newalrm->minute = min;
	return newalrm;
}
int tick_alarm(int curr_state){
	int key = 0;
	if (focus == _alarm){ // only get key when the screen is at clock
		if(key_buf != 0 ){
			key = key_buf;
			key_buf = 0;
		}
	}
	switch(curr_state){
		case(init_alarm):
			curr_state = check_alarm;
			break;
		case(check_alarm):
			if (key == set){
				 curr_state = edit_alarm;
				 alarm_curr_edit = alrm_hr;
				 break;
			}
			if (clk->hour == alarm->hour && clk->minute == alarm->minute
				&& alarm_flag != 0) curr_state = play_sound;
			break;
		case(edit_alarm):
			if (key == set){
				curr_state = check_alarm;
				alarm_curr_edit = alarm_edit_none;
				break;
			}
		case(play_sound):
			if (alarm_flag == 0) curr_state = check_alarm;
			break;
		default:
			break;
	}
	switch(curr_state){
		case(init_alarm):
			break;
		case(check_alarm):
			alarm_curr_edit = alarm_edit_none;
			sound_off();
			break;
		case(edit_alarm):
			if(alarm_blink_flag_ctr >= ALARM_BLINK_FLAG_CTR){
				toggle_blink_flag();
				alarm_blink_flag_ctr=0;
			}
			else alarm_blink_flag_ctr++;
			switch(key){
				case (up):
					increment_alarm(alrm_curr_edit, alrm);
					break;
				case (down):
					decrement_alarm(alrm_curr_edit, alrm);
					break;
				case (left):
				case (right):
					move_cursor(alrm_curr_edit);
					break;
				case (mode):
				  break;
			}

			break;
		case(play_sound):
			// temporary
			flip_sound();
			break;
		default:
			break;
	}
	return curr_state;
}
void flip_sound(){
	FLIP_BIT(ALARM_PORT, ALARM_GATE);
}

void sound_off(){
	CLR_BIT(ALARM_PORT, ALARM_GATE);
}
void increment_alarm(int what, Alarm * alrm) {
	if (what == alrm_hr)
		alrm->hour = (alrm->hour+1)%24;
	else if (what == alrm_min && alrm->minute == 59) {
		alrm->hour = (alrm->hour+1)%24;
		alrm->minute = 00;
	}
	else if (what == alrm_min)
		alrm->minute++;
}
void decrement_alarm(int what, Alarm * alrm) {
	if (what == alrm_hr)
		alrm->hour = (alrm->hour-1)%24;
	else if (what == alrm_min && alrm->minute == 0) {
		alrm->hour = (alrm->hour-1)%24;
		alrm_min = 59;
	}
	else if (what == alrm_min)
		alrm->minute--;
}
void move_cursor(int what) {
	if (what == alrm_hr)
		alrm_curr_edit = alrm_min;
	else if (what == alrm_min)
		alrm_curr_edit = alrm_hr;
}

char* alarm_time(Alarm * alrm, int alarm_curr_edit) {
	int size = 18;
	char * s = malloc(sizeof(char)*size);
	if (alarm_curr_edit == alrm_hr && blink_flag == 0)
	sprintf(s, "  :%02d            ", alrm->minute);
	else if (alarm_curr_edit == alrm_min && blink_flag == 0)
	sprintf(s, "%02d:            ", alrm->hour);
	else
	sprintf(s, "%02d:%02d           ", alrm->hour, alrm->minute);
	return s;
}

// NEED TO ENUM alrm_curr_edit in .h?
void display_alarm(Alarm * alrm, int alrm_curr_edit){
	lcd_pos(0,0);
	char * s;
	s = alarm_time(alrm, alarm_curr_edit);
	lcd_puts2(s);
	lcd_pos(1,0);		// cursor at position 0 line 0
	free(s);
	char c2[18];
	sprintf(c2, "EDIT ALARM      ");
	lcd_puts2(c2);
}
