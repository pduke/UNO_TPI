// tiny10_blink.c 
//
// Demo program to blink a LED
//
//		by Paul Duke 2014

#include <avr/io.h>
#include <util/delay.h>

#define LED_PIN 	0																	// PB2

void main() {
	// set clock to 8MHz
	CCP = 0xD8;																				// enable protected change
	CLKPSR = 0;               												// set 8MHz prescaler to 1

	// power reduction
	ACSR = _BV(ACD);																	// disable analog comparator
	DIDR0 = 0x0F;																			// disable digital inputs
	PRR = _BV(PRADC);																	// disable adc

	// setup port
	DDRB = 0x0F;																			// all output

	// loop
	for(;;){
		PINB |= _BV(LED_PIN);														// toggle
		_delay_ms(500);
	}
}

