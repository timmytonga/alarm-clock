/*
 * clock.h
 *
 * Created: 4/25/2019 11:19:37 PM
 *  Author: nguyenhangthien
 */ 


#ifndef CLOCK_H_
#define CLOCK_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int second, minute, hour;
	int day, month, year;
	char military;
} Clock;

/* Clock's interface */
Clock * clock_init(int day, int month, int year, int hour,
int minute, int second, int military);		//returns clock initialized 2/8/19, military true
								//00:00:00 time by default
char is_leapyear(Clock *);
void increment_year(Clock *);
void increment_month(Clock *);
void increment_day(Clock*);
void increment_hour(Clock *);
void increment_minute(Clock *);
void increment_second(Clock *);

void decrement_year(Clock *);
void decrement_month(Clock *);
void decrement_day(Clock*);
void decrement_hour(Clock *);
void decrement_minute(Clock *);
void decrement_second(Clock *);

void keep_time(Clock *);
void toggle_military(Clock *);

#endif /* CLOCK_H_ */