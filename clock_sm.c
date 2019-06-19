/*
 * clock_sm.c
 *
 * Created: 6/11/2019 9:47:58 PM
 *  Author: nguyenhangthien
 */ 

#include "clock_sm.h"


int tick_clk(int curr_state){
	/* tick function to be called to manage main clk every period 
	we first write it to keep time. returns the next state
	*/
	int key = 0;
	if (focus == _clock){ // only get key when the screen is at clock
		if(key_buf != 0 ){ 
			key = key_buf;
			key_buf = 0;
		}
	}
	switch (curr_state){
		case (init):
			curr_state = keep;
			break;
		case (keep): // check button pressed
			if (key == edit){
				curr_state = set;
				curr_edit = hour;
			}
			else
				curr_state = keep;
			break;
		case (set):
			if (key == edit){
				curr_state = keep;
				curr_edit = none;
			}
			else
				curr_state = set;
			break;
		default:
			curr_state = init;
			break;
	}
	switch (curr_state){
		case(init): // how to get here??? 
			break;
		case (keep):
			//display_time(clk, none);
			if (keep_time_counter >= KEEPTIME_COUNTER_CHECK){
				keep_time(clk);
				keep_time_counter = 0;
			}
			else keep_time_counter++;
			if (key == tog_mil) toggle_military(clk);
			break;
			
		case (set):
			//display_time(clk, curr_edit);
			toggle_blink_flag();
			switch (key) {
				case (up):
					increment(curr_edit, clk);
					break;
				case (down):
					decrement(curr_edit, clk);
					break;
				case (left):
					cycle_left();
					break;
				case (right):
					cycle_right();
					break;
				case (tog_mil):
					toggle_military(clk);
					break;
				default:
					break;	
			}
			break;
		default:
			break;
	}
	if (key == tog_alarm) alarm_flag = (alarm_flag + 1)%2;
	return curr_state;
}


char* get_time(Clock* self, int edit_f) {
	int size = 17;
	char * s = malloc(sizeof(char)*size);
	char * ampmmil = "  ";
	int milhour = self->hour;
	if (!self->military){
		if (milhour < 12){
			ampmmil = "am";
			if (milhour == 0)
			milhour = 12;
		}
		else if (milhour == 12){
			ampmmil = "pm";
		}
		else{ // milhour >12
			milhour %= 12;
			ampmmil = "pm";
		}
	}
	if (edit_f == none)
	sprintf(s, "%02d:%02d:%02d%s", milhour, self->minute, self->second, ampmmil);
	else if (edit_f == hour && blink_flag == 0)
	sprintf(s, "  :%02d:%02d%s", self->minute, self->second, ampmmil);
	else if (edit_f == minute && blink_flag == 0)
	sprintf(s, "%02d:  :%02d%s", milhour, self->second, ampmmil);
	else if (edit_f == second && blink_flag == 0)
	sprintf(s, "%02d:%02d:  %s", milhour, self->minute, ampmmil);
	else if (edit_f == mil && blink_flag == 0)
	sprintf(s, "%02d:%02d:%02d  ", milhour, self->minute, self->second);
	else
	sprintf(s, "%02d:%02d:%02d%s", milhour, self->minute, self->second, ampmmil);
	return s;
}

char* get_date(Clock* self, int edit_f) {
	int size = 17;
	char * s = malloc(sizeof(char)*size);
	if (edit_f == none)
	sprintf(s, "%02d/%02d/%02d", self->month, self->day, self->year%100);
	else if (edit_f == month && blink_flag == 0)
	sprintf(s, "  /%02d/%02d", self->day, self->year%100);
	else if (edit_f == day && blink_flag == 0)
	sprintf(s, "%02d/  /%02d", self->month, self->year%100);
	else if (edit_f == year && blink_flag == 0)
	sprintf(s, "%02d/%02d/  ", self->month, self->day);
	else
	sprintf(s, "%02d/%02d/%02d", self->month, self->day, self->year%100);
	return s;
}

void display_time(Clock* clk, int edit_f){
	lcd_pos(0,0);		// cursor at position 0 line 0
	char * s;
	s = get_time(clk, edit_f);
	lcd_puts2(s);
	lcd_pos(1,0);		// cursor at position 0 line 0
	free(s);
	char * c;
	char c2[18]; 
	c = get_date(clk, edit_f);
	char * alrm = "    ";
	if (alarm_flag != 0) alrm = "alrm";
	sprintf(c2, "%s    %s", c, alrm);
	lcd_puts2(c2);
	free(c);
}
void run_clock(Clock* clk) {
	display_time(clk, none);
	keep_time(clk);
}

void cycle_right() {
	if (curr_edit == month)
	curr_edit = day;
	else if (curr_edit == day)
	curr_edit = year;
	else if (curr_edit == year)
	curr_edit = hour;
	else if (curr_edit == hour)
	curr_edit = minute;
	else if (curr_edit == minute)
	curr_edit = second;
	else if (curr_edit == second)
	curr_edit = month;
	else
	curr_edit = hour;
}

void cycle_left() {
	if (curr_edit == month)
	curr_edit = second;
	else if (curr_edit == day)
	curr_edit = month;
	else if (curr_edit == year)
	curr_edit = day;
	else if (curr_edit == hour)
	curr_edit = year;
	else if (curr_edit == minute)
	curr_edit = hour;
	else if (curr_edit == second)
	curr_edit = minute;
	else
	curr_edit = hour;
}



void increment(int edit_f, Clock* clk) {
	if (edit_f == month)
	increment_month(clk);
	else if (edit_f == day)
	increment_day(clk);
	else if (edit_f == year)
	increment_year(clk);
	else if (edit_f == hour)
	increment_hour(clk);
	else if (edit_f == minute)
	increment_minute(clk);
	else if (edit_f == second)
	increment_second(clk);
	else if (edit_f == mil)
	toggle_military(clk);
}

void decrement(int edit_f, Clock* clk)
{
	if (edit_f == month)
	decrement_month(clk);
	else if (edit_f == day)
	decrement_day(clk);
	else if (edit_f == year)
	decrement_year(clk);
	else if (edit_f == hour)
	decrement_hour(clk);
	else if (edit_f == minute)
	decrement_minute(clk);
	else if (edit_f == second)
	decrement_second(clk);
	else if (edit_f == mil)
	toggle_military(clk);
}