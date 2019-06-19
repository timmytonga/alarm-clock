/*
 * clock.c
 *
 * Created: 4/25/2019 11:21:08 PM
 *  Author: nguyenhangthien
 */ 

#include "clock.h"

Clock * clock_init(int day, int month, int year, int hour, 
					int minute, int second, int military){
	Clock * clk = malloc(sizeof(Clock));
	clk->day = day;
	clk->month = month;
	clk->year= year;
	clk->hour = hour;
	clk->minute = minute;
	clk->second = second;
	clk->military = military;
	return clk;
}

char is_leapyear(Clock* self) {
	if ((self->year)%4 == 0) {
		if ((self->year%100 == 0)) {
			if ((self->year%400) == 0)
				return 1;
			else
				return 0;
		}
		else
			return 1;
	}
	else {
		return 0;
	}
}

void increment_year(Clock *self) {
	if (self->month == 2 && self->day == 29) self->day = 28;
	self->year++;
}

void increment_month(Clock *self) {
	if (self->month == 12) {
		self->month = 1;
		increment_year(self);
	}
	if (self->day == 30){
		if (self->month == 1){
			if (is_leapyear(self))
				self->day = 29;
			else self->day = 28;
		}
	}
	if (self->day == 31){
		if (self->month == 1){
			if (is_leapyear(self))
			self->day = 29;
			else self->day = 28;
		}
		else if (self->month == 3 || self->month == 5 || self->month == 7 || self->month == 8 || self->month == 10  )
			self->day = 30;
	}
	self->month++;
}

void increment_day(Clock* self) {
	if (self->day == 31) {
		increment_month(self);
		self->day = 1;
	}
	else if (self->day == 30) {
		if (self->month==4||self->month==6||self->month==9||self->month==11) {
			increment_month(self);
			self->day = 1;
		}
		else
			self->day++;
	}
	else if (self->day == 28 && self->month == 2) {
		if (!is_leapyear(self)) {
			increment_month(self);
			self->day = 1;
		}
		else {
			self->day++;
		}
	}
	else if (self->day == 29 && self->month == 2) {
		increment_month(self);
		self->day = 1;
	}
	else
		self->day++;
}

void increment_hour(Clock* self) {
	if (self->hour == 23){
		increment_day(self);
		self->hour = 0;
	}
	else
		self->hour++;
}

void increment_minute(Clock* self) {
	if (self->minute == 59){
		increment_hour(self);
		self->minute = 0;
	}
	else
		self->minute++;
}

void increment_second(Clock *self) {
	if (self->second == 59) {
		increment_minute(self);
		self->second = 0;
	}
	else
		self->second++;
}

void decrement_year(Clock *self) {
	if (self->month == 2 && self->day == 29) self->day = 28;
	self->year--;
}
void decrement_month(Clock* self) {
	if (self->month == 1) {
		decrement_year(self);
		self->month = 12;
	}
	if (self->day == 30){
		if (self->month == 3){
			if (is_leapyear(self))
				self->day = 29;
			else self->day = 28;
		}
	}
	else if (self->day == 31){
		if (self->month == 3){
			if (is_leapyear(self))
				self->day = 29;
			else self->day = 28;
		}
		else if (self->month == 5 || self->month == 7 || self->month == 9 || self->month == 10 || self->month == 12  )
			self->day = 30;
	}
	self->month--;
	
}
void decrement_day(Clock* self) {
	if (self->day == 1) {
		if (self->month == 1||self->month == 2||self->month == 4||self->month == 6
			||self->month == 8||self->month == 9||self->month == 11) {
			self->day = 31;
			decrement_month(self);
		}
		else if (self->month == 3){
			if (is_leapyear(self))
				self->day = 29;
			else self->day = 28;
				decrement_month(self);
		}
	}
	else
		self->day--;
}
void decrement_hour(Clock * self) {
	if (self->hour == 0) {
		decrement_day(self);
		self->hour = 23;
	}
	else
		self->hour--;
}
void decrement_minute(Clock * self) {
	if (self->minute == 0) {
		decrement_hour(self);
		self->minute = 59;
	}
	else
		self->minute--;
}
void decrement_second(Clock * self) {
	if (self->second == 0) {
		decrement_minute(self);
		self->second = 59;
	}
	else
		self->second--;
}

void keep_time(Clock *self) {
	// avr_wait();
	increment_second(self);
}

void toggle_military(Clock *self) {
	if (self->military == 0)
		self->military = 1;
	else
		self->military = 0;
}


