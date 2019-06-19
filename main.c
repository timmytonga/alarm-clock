/*
 * project5.c
 *
 * Created: 6/11/2019 8:21:01 PM
 * Author : nguyenhangthien
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "avr.h"
#include "keypad.h"
#include "keypad_sm.h"
#include "lcd.h"
#include "tasks.h"
#include "clock.h"
#include "clock_sm.h"
#include "alarm_sm.h"
#include "ui_manager.h"
#include "globals_common.h"

#define TASKS_PERIOD	100 // in ms
#define NUMTASKS		4

enum screens focus; // this flag helps the display to decide which mode the user is on

/* GLOBAL VAR FOR CLOCK */
unsigned char blink_flag = 0; // global var to implement blink. If 0 then space if not 0 then display normally
enum clock_state clk_state = init;
enum time_type curr_edit;
Clock * clk;
int keep_time_counter = 0; 

/* GLOBAL VAR FOR KEYPAD */
enum button key_pressed;
int key_buf; // buffer for stuff to use keypressed
int tb_remaining_toggle = 0; // for blink led test tickfct

/* GLOBAL VAR FOR TASKS/SCHEDULER */
// priority should be keypad -> uimanager -> clocktask -> alarmtask
enum task_list {KeyPad=0, UiManagerTask = 1, ClockTask=2, AlarmTask=3};
task tasks[NUMTASKS]; // make a tasks array
void init_tasks();

/* GLOBAL VAR FOR ALARM */
unsigned char alarm_flag = 0; // 0 means alarm is not set; 1 means is set
Alarm * alarm; // default 8 am
enum alarm_edit alarm_curr_edit; 
unsigned char alarm_blink_flag_ctr = 0;

/* ISR FUNCTION: CTC IS ENABLED SO NO NEED TO CLEAR TNCT */
ISR(TIMER1_COMPA_vect){ // timer interrupt handler function 
	run_tasks(tasks, TASKS_PERIOD, NUMTASKS);
}

int main(void)
{
	key_buf = 0; // set the buffer to 0
    init_keypad();
    lcd_init(); lcd_clr();
    clk = clock_init(27,2,2020,23,59,0,0); // day, month, year, hour, minute, second, military
	TimerSet(TASKS_PERIOD); // set interrupt to occur every 500 ms
	TimerOn();
	/* setup ports */
	SET_BIT(DDRA, 0);
	init_tasks();
	/* initialize alarm */
	alarm = alarm_init(0,0);
	focus = _clock;
	curr_edit = none;
	// sleep to save energy while waiting next cycle
	sleep_enable();
	while (1)
	{
		sleep_cpu();
	}
}

void init_tasks(){
	// make_tasks(start_state, period, tick_fct)
	tasks[KeyPad] = *make_tasks(0, KEYPADSM_PERIOD, tick_keypad);
	tasks[ClockTask] = *make_tasks(init, CLOCKSM_PERIOD, tick_clk);
	tasks[AlarmTask] = *make_tasks(init_alarm, ALARMSM_PERIOD, tick_alarm);
	tasks[UiManagerTask] = *make_tasks(_init, UI_MANAGER_SM_PERIOD, tick_ui_manager);
}



