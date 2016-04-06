//	pin_io_macros.h
//
//	Makes it easy to define pins as logical names and then use those names
//  throughout the program. Not the most efficient method for multiple pins
//	on the same port but otherwise makes coding easier and more transportable.
//
//		by Paul Duke	2012

#ifndef PIN_IO_MACROS 
	#define PIN_IO_MACROS
	// pin I/O helper macros
	//
	#define INPUT2(port,pin) DDR ## port &= ~_BV(pin) 
	#define OUTPUT2(port,pin) DDR ## port |= _BV(pin) 
	#define CLEAR2(port,pin) PORT ## port &= ~_BV(pin) 
	#define SET2(port,pin) PORT ## port |= _BV(pin) 
	#define CLEAR_PULLUP2(port,pin) PUE ## port &= ~_BV(pin) 
	#define SET_PULLUP2(port,pin) PUE ## port |= _BV(pin) 
	#define TOGGLE2(port,pin) PORT ## port ^= _BV(pin) 
	#define READ2(port,pin) ((PIN ## port & _BV(pin))?1:0)
	#define STATE2(port,pin) ((DDR ## port & _BV(pin))?1:0)
	#define PIN_NAME2(port,pin) PIN_NAME3(P ## port ## pin)
	#define PIN_NAME3(name) #name
	#define PIN_NUMBER2(port, pin) pin
	//
	#define INPUT(x) INPUT2(x) 
	#define OUTPUT(x) OUTPUT2(x)
	#define CLEAR(x) CLEAR2(x)
	#define SET(x) SET2(x)
	#define TOGGLE(x) TOGGLE2(x)
	#define READ(x) READ2(x)
	#define STATE(x) STATE2(x)
	#if defined(__ATTINY10__)
		#define PULLUP_ON(x) INPUT2(x); SET_PULLUP2(x)
		#define PULLUP_OFF(x) INPUT2(x); CLEAR_PULLUP2(x)
	#else
		#define PULLUP_ON(x) INPUT2(x); SET2(x)
		#define PULLUP_OFF(x) INPUT2(x); CLEAR2(x)
	#endif
	#define PIN_NAME(x) PIN_NAME2(x)
	#define PIN_NUMBER(x) PIN_NUMBER2(x)
#endif


