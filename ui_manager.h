/*
 * ui_manager.h
 *
 * Created: 6/13/2019 4:03:15 PM
 *  Author: nguyenhangthien
 */ 


#ifndef UI_MANAGER_H_
#define UI_MANAGER_H_

#include "globals_common.h"
#include "avr.h"
#include "clock_sm.h"
#include "alarm_sm.h"

#define UI_MANAGER_SM_PERIOD 250

/* variables defined in main */
extern enum screens focus; // right now only alarm and clock
extern int key_buf;
extern Clock * clk;			
extern enum time_type curr_edit;
extern Alarm * alarm;
extern enum alarm_edit alarm_curr_edit; 

/* manage displaying tasks */

int tick_ui_manager(int curr_state);

#endif /* UI_MANAGER_H_ */