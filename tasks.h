/* tasks class for implementing threading style in AVR programming */
#ifndef TASKS_H
#define TASKS_H

#include <avr/interrupt.h>   // for timer interrupt
#include "avr.h"
#include <stdlib.h>

/* we configure interrupts and timer ISR here but where is better ? */
void TimerSet(int period_in_ms); // set the period that the ISR will be invoked to call the scheduler
void TimerOn(); // start the timer and the ISR will be called

/* user must writes a Tick Function to pass to the task
    the function should not take too long per tick --> what is this exactly?*/
typedef struct task{
    int             state;  // Task's current state
    unsigned long   period; // Task period
    unsigned long   elapsedTime; // Time elapsed since last task tick
    int (*TickFct)(int);    // task tick function where arg is current state
                            // and should returns the next state
    char            toRun;  // if 0 then does not schedule task, else run it
} task;

/* this function takes an array of tasks
    user specifies an array of tasks and also period gcd of tasks
    set the timer to tick per GCD
    for each tasks, if the desired period has elapsed, run the tick function
    ( just using round-robin) -- what happen if missed-deadline? */
void run_tasks(task* taskarray, int gcd, int numtasks);
void run_scheduler(); // to do
task* make_tasks(int state, unsigned long period, int (*TickFct)(int));

/* maybe: schedule struct, add_tasks, remove_tasks, run_scheduler, pause_scheduler
            set_period, change_period, compute gcd, etc. */
void add_tasks(task* new_task, task* curr_arr_of_tasks);

#endif
