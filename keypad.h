#ifndef _KEYPAD_H

#include "avr.h"

#define PORTKEYPAD PORTC
#define DDRKEYPAD DDRC
 
int is_pressed(int r, int c); /* check if certain key in row r and col c is pressed*/

int get_key(); /* loop through and return which button is pressed */

void blink(int); // blink LED at portb0

void init_keypad();

#endif /* _KEYPAD_H*/