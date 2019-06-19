/*
 * project2.c
 *
 * Created: 4/23/2019 10:19:25 AM
 * Author : thien & thao
 */ 

#include <avr/io.h>
#include "keypad.h"
#include "avr.h"
#include "lcd.h"
#include "clock.h"
int keep_times = 44;
int set_times = 20;
int blink_flag = 0; // global var to implement blink. If 0 then space; 
					// if not 0 then display normally
enum clock_state {init, keep, set} clk_state;
enum time_type {none, hour, minute, second, mil, day, month, year} curr_edit;
enum button {up = 2, down = 10, left = 5, right = 7, edit = 6, tog_mil = 13} key;
char* get_time();			//format hh:mm:ss, hh depending on char military
char* get_date();			//format mm/dd/yy
void run_clock(Clock* clk);
void state_machine(Clock * clk);
void display_time(Clock* clk, int edit_f);
void cycle_right(); // cycle right edit set
void cycle_left();  // same
void toggle_blink_flag();
void increment(int edit_f, Clock * clk);
void decrement(int edit_f, Clock * clk);
// military time at 00
// decrement march to february 
// december 31?
int main(void)
{
	SET_BIT(DDRA, 0);
	// PORTS: rows are from 0-3; columns are from 4-7
	//SET_BIT(DDRB, 0);	// LED output for testing
	DDRC = 0x00;		// set port C to input
	PORTC = 0x0f;		// set port C row wires to weak 1 and column wires to
						// no connection
	lcd_init();
	//int day, int month, int year, int hour, int minute, int second, int military
	Clock * clk = clock_init(27,2,2020,23,59,0,0);
	clk_state = init;
	lcd_clr();
	curr_edit = hour;
    /* Replace with your application code */
    while (1) 
    {
		state_machine(clk);
	}
}

void state_machine(Clock * clk){
	switch(clk_state) {
		case (init):
			clk_state = keep;
			break;
		case (keep):
			key = 0;
			for (int i = 0; i < keep_times; i++){
				int tmp = get_key();
				if (tmp !=0) key = tmp;
				avr_wait(25);
			}
			blink(key);
			if (key == edit)
				clk_state = set;
			else
				clk_state = keep;
			break;
			
		case (set):
			key = 0;
			for (int i = 0; i < set_times; i++){
				int tmp = get_key();
				if (tmp !=0) key = tmp;
				avr_wait(25);
			}
			blink(key);
			
			if (key == edit)
				clk_state = keep;
			else
				clk_state = set;
			break;
			
		default:
				clk_state = init;
			break;
	}
	switch (clk_state) {
		case (init):
			curr_edit = hour; /* default we start editing at hour */
			break;
		case (keep):
			run_clock(clk);
			if ( key == tog_mil) toggle_military(clk);
			break;
		case (set):
			display_time(clk, curr_edit);
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
	c = get_date(clk, edit_f);
	lcd_puts2(c);
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

void toggle_blink_flag(){
	if (blink_flag == 0)
	blink_flag = 1;
	else
	blink_flag = 0;
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