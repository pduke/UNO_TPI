// tiny10_toggle.c 
//
// Make a debounced toggle switch from a simple tactile switch.
// Also includes an inverted output pin and a tri-state pin. All
// pins toggle states when the button is pressed. Software debouncing
// provides a 50ms bounce period.
//
//		by Paul Duke 2016

#include <avr/io.h>
#include <avr/interrupt.h>

#define SWITCHED_PIN 			0													// the toggle, on/off
#define INVERTED_PIN 			1													// inverted toggle, off/on
#define TRI_STATE_PIN			2													// tri-state toggle, on/tri-state
#define BUTTON_PIN				3													// debounced button

#define BOUNCE_PERIOD_MS 50
#define BUTTON_DOWN 			0

// button debouncing states
#define IDLE 				0
#define PRESSED 		1
#define WAIT 				2
#define RELEASED 		3

// timer interrupt increments ms counter
volatile uint32_t msNow;
ISR(TIM0_COMPA_vect, ISR_BLOCK){
	msNow ++;
}

void main() {
	// setup ports
	DDRB = _BV(SWITCHED_PIN) | _BV(INVERTED_PIN)			// outputs
			| _BV(TRI_STATE_PIN);
	PORTB = _BV(INVERTED_PIN) | _BV(TRI_STATE_PIN);		// turn on pins
	PUEB = _BV(BUTTON_PIN);														// enable pullup
	
	// start ms timer
	TCCR0A = 0;																				// no output
	TCCR0B = _BV(WGM02) | _BV(CS00);									// ctc, no prescaling
	OCR0A = 1000;																			// 1MHz/1000 = 1ms
	TIMSK0 = _BV(OCIE0A);															// interrupt on match
	sei();																						// enable interrupts

	// debouncing variables
	uint16_t msElapsed;																// elapsed time in ms
	uint32_t msStamp = msNow;													// timestamp
	uint8_t debouncing = 0;														// debounce flag

	// loop debouncing the button based on elapsed time
	// instead of using delays
	for(;;){
		msElapsed = msNow - msStamp;										// calc elapsed time
		if((PINB & _BV(BUTTON_PIN)) == BUTTON_DOWN){		// button's down
			if(debouncing == IDLE){												// if not debouncing
				msStamp = msNow;														// time stamp
				debouncing = PRESSED;												// and start debounce
			} else {
				if(debouncing == PRESSED){									// still debouncing
					if(msElapsed > BOUNCE_PERIOD_MS){					// button's still down and debounce time elapsed
						if(PINB & _BV(SWITCHED_PIN)){
							PORTB &= ~_BV(SWITCHED_PIN);					// toggle pins
							PORTB |= _BV(INVERTED_PIN);
							DDRB &= ~_BV(TRI_STATE_PIN);					// set to tri-state (input)
							PORTB &= ~_BV(TRI_STATE_PIN);					// turn off
						} else {
							PORTB |= _BV(SWITCHED_PIN);						// toggle pins
							PORTB &= ~_BV(INVERTED_PIN);
							DDRB |= _BV(TRI_STATE_PIN);						// set output
							PORTB |= _BV(TRI_STATE_PIN);					// turn on
						}
						debouncing = WAIT;											// wait for button up
					}
				}
			}
		} else {																				// button's up
			if(debouncing == WAIT){												// if not debouncing
				msStamp = msNow;														// time stamp
				debouncing = RELEASED;											// and start debounce
			} else {
				if(debouncing == RELEASED){									// still debouncing
					if(msElapsed > BOUNCE_PERIOD_MS){					// button's still up and debounce time elapsed
						debouncing = IDLE;											// wait for button down again
						msNow = 0;															// reset time to prevent overflow
						msStamp = 0;														// time stamp
					}
				}
			}
		}
	}
}

