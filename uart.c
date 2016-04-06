//	uart.h
//
// This is basicall the old nerdkits uart code cleaned up.
//
//		by Paul Duke 	2012

#ifndef __UART_H
#define __UART_H

#include <inttypes.h>
#include <stdio.h>

FILE mystream;

void uart_init();
void uart_write(char x);
uint8_t uart_char_is_waiting();
char uart_read();

int uart_putchar(char x, FILE *stream);
int uart_getchar(FILE *stream);

// uart.c

#include <stdio.h>
#include <stdlib.h>

#include <avr/io.h>
#include <inttypes.h>

void uart_init() {
	// set baud rate
	UCSR0A = 0;
	UBRR0L = (F_CPU/(16*BAUD))-1;
	// enable uart RX and TX
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	// set 8N1 frame format
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);

	// set up STDIO handlers so you can use printf, etc
	fdevopen(&uart_putchar, &uart_getchar);
}

void uart_write(char x) {
	// wait for empty receive buffer
	while ((UCSR0A & (1<<UDRE0))==0);
	// send
	UDR0 = x;
}

uint8_t uart_char_is_waiting() {
	// returns 1 if a character is waiting
	// returns 0 if not
	return (UCSR0A & (1<<RXC0));
}

char uart_read() {
	// wait
	while(!uart_char_is_waiting());
	char x = UDR0;
	return x;
}

int uart_putchar(char c, FILE *stream) {
  uart_write(c);
  return 0;
}
    
int uart_getchar(FILE *stream) {
  int x = uart_read();
  return x;
}

#endif
