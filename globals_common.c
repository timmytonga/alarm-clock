/*
 * globals_common.c
 *
 * Created: 6/13/2019 11:58:01 PM
 *  Author: nguyenhangthien
 */ 
#include "globals_common.h"

void toggle_blink_flag(){
	if (blink_flag == 0)
	blink_flag = 1;
	else
	blink_flag = 0;
}