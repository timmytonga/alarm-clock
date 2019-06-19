#include "tasks.h"

unsigned long _bucket_size  = 0;
task* make_tasks(int state, unsigned long period, unsigned long elapsedTime,
                  int (*TickFct)(int))
{
    task* newtask = malloc(sizeof(task));
    newtask.state = state;
    newtask.period = period;
    newtask.elapsedTime = elapsedTime;
    newtask.TickFct = TickFct;
    newtask.toRun = 1; // let task run as default
    return newtask;
}
void run_tasks(task* tasks, int gcd, int tasksNum){ /* run the lists of tasks in roundrobin */
    // set timer to tick per gcd by modifying a global variable that triggers
    //      the timer ISR at different times
    // For each task, call the task's tickfunction if task's period is up
    for (unsigned char i = 0; i < tasksNum; i++){
        if (tasks[i].elapsedTime >= tasks[i].period){
            // Task is ready to tick so call its tick function
            tasks[i].state = tasks[i].TickFct(tasks[i].state);
            tasks[i].elapsedTime = 0; // reset elapsed time
        }
        tasks[i].elapsedTime += gcd;
    }
}

void run_scheduler(task* tasks, int timer_period_gcd){
    int numTasks = sizeof(tasks)/sizeof(tasks[0]); // number of tasks per this schedule
    run_tasks(tasks, timer_period_gcd, numTasks); // run through the list of tasks for this time period
}

void TimerSet(int period_in_ms){
    // turn on ISR
    // calculate the appropriate time and set the global variable accordingly
    // ???
    _bucket_size = 65535-(XTAL_FRQ/1024)*period_in_ms;
    TCNT1 = 65535-(XTAL_FRQ/1024)*period_in_ms; // what value?
    TCCR1A = 0x00;
    TIMSK = (1<< TOIE1); // enable timer1 overflow interrupt
    // make sure to reset TCNT1 in ISR.
}

void TimerOn(){
    // turn on the timer and enable interrupt
    TCCR1B = (1<<CS10) | (1 << CS12); // timer mode with 1024 prescaler
    sei(); // enable global interrupts by setting global interrupt enable bit in SREG
}
