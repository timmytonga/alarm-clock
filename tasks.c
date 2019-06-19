#include "tasks.h"

unsigned long _bucket_size  = 0;
task* make_tasks(int start_state, unsigned long period, int (*TickFct)(int))
{
    task* newtask = malloc(sizeof(task));
    newtask->state = start_state;
    newtask->period = period;
    newtask->elapsedTime = period;
    newtask->TickFct = TickFct;
    newtask->toRun = 1; // let task run as default
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


void TimerSet(int period_in_ms){
    //_bucket_size = 65535-(XTAL_FRQ/1024)*period_in_ms;
   // TCNT1 = 65535-(XTAL_FRQ/1024)*period_in_ms; // what value?
	OCR1A = (XTAL_FRQ/64000)*period_in_ms;
    TIMSK = (1<< OCIE1A); // enable timer1 compare interrupt
}

void TimerOn(){
    // turn on the timer and enable interrupt
	// timer mode with 1024 prescaler as well as CTC 
	TCCR1A = 0x00;
    TCCR1B = (1<<WGM12)|(1<<CS10) |(1 << CS11); 
	TCNT1 = 0;
    sei(); // enable global interrupts by setting global interrupt enable bit in SREG
}
