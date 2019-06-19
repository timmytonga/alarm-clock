#include "keypad.h"
#define CRYSTAL 8 // set this to 1 if crystal is turned on

// return 0 if no button is pressed
// return 1-15 for button pressed
void blink (int times) { // blink LED at port A0
	int short_wait = 1000;
	/*
	SET_BIT(PORTB, 0);
	avr_wait(long_wait);
	CLR_BIT(PORTB, 0);
	avr_wait(short_wait);
	*/
	if (times > 0){
		for (int i=0; i<times; i++) {
			avr_wait(short_wait);
			SET_BIT(PORTA, 0);
			avr_wait(short_wait);
			CLR_BIT(PORTA, 0);
		}
	}
}
int get_key() {
	int r,c ;
	for (r=0; r<4; r++) {
		for (c=0; c<4; c++) {
			if (is_pressed(r,c)) {
				return (r*4+c)+1;  // range from 1 to 16
			}
		}
	}
	return 0;
}

int is_pressed(int r, int c) {
	SET_BIT(DDRC, c+4);		// set c to output (default to strong 0) 
	CLR_BIT(PORTC, c+4);	// output strong 0
	int pressed = 0;		// flag for return
	if ( GET_BIT(PINC,r) == 0 ) // if (r,c) pressed then pincr == 0
		pressed = 1;		// pressed
	CLR_BIT(DDRC, c+4);		// back to 0 (input) --> back to no connection
	return pressed;
}

void init_keypad() {
	DDRKEYPAD = 0x00;		// set port C to input
	PORTKEYPAD = 0x0f;		// set port C row
}