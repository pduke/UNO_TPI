//	lcd_4_bit.h
//
// Modified Nerdkits LCD.c to use ready/busy flag, provide line wrap,
// scrolling, blinking cursor, dynamic pin assignments, and load patterns
// into CGRAM. 
//
// Currently supports 4x20 or 2x16 lcd displays. Pass lines and width
// in lcd_init(). 
//
//		by Paul Duke	2013
//

#ifndef __LCD_4_BIT_H
#define __LCD_4_BIT_H

#include <stdio.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <inttypes.h>
#include <stdio.h>
#include <util/delay.h>
#include "pin_io_macros.h"

// these functions used externally
void lcd_init(uint8_t lines, uint8_t chars);
void lcd_clear_and_home(void);
void lcd_goto_position(uint8_t row, uint8_t col);
void lcd_cursor_blink(bool state);
void lcd_set_pattern(uint8_t index, uint8_t line1, uint8_t line2, uint8_t line3, uint8_t line4, uint8_t line5, uint8_t line6, uint8_t line7, uint8_t line8);
void lcd_write_data(char c);
void lcd_write_command(char c);
int lcd_putchar(char x, FILE *stream);
// To enable fprintf output to lcd, use lcd_putchar with:
//	FILE lcd_stream;
// 	fdev_setup_stream(&lcd_stream, lcd_putchar, 0, _FDEV_SETUP_WRITE); 

// these functions typically used only internally
static inline void lcd_set_type_data(void);
static inline void lcd_set_type_command(void);
void lcd_write_nibble(char c);
void lcd_write_byte(char c);
char lcd_read_nibble(void);
char lcd_read_byte(void);
void lcd_set_data_pins_output(void);
void lcd_set_data_pins_input(void);
void lcd_busy_wait_4_bit(void);
void lcd_scroll_up(void);

// define ports, pins
//		The LCD read/write pin must be set as this version of the code
//		reads the busy/read flag instead of using timeouts for regular
//		text I/O. With the ready flag comes the current cursor position
//		that is used to wrap lines and scroll the display.
//
//		Removed unused functions and added a pattern loading funciton.
//
// UC-204 LCD Display pin-outs
//
//	1	GND
//	2	VCC
//	3	VO 	(contrast resistor to GND)
//	4	RS 	(register select)
//	5	R/W	(read/write)
//	6	E	(enable)
//	7	DB0
//	8	DB1
//	9	DB2
//	10	DB3
//	11	DB4
//	12	DB5
//	13	DB6
//	14	DB7
//	15	LED+(backlight)
//	16	LED-
//

#define LCD_CHECK LCD_RS
#define LCD_OFFSET 0x40
uint8_t lcd_lines = 4;
uint8_t lcd_chars = 20;
volatile uint8_t DDRAM_address;

void lcd_set_data_pins_output(){
	OUTPUT(LCD_DB4);	CLEAR(LCD_DB4);
	OUTPUT(LCD_DB5);	CLEAR(LCD_DB5);
	OUTPUT(LCD_DB6);	CLEAR(LCD_DB6);
	OUTPUT(LCD_DB7);	CLEAR(LCD_DB7);
	_delay_us(1);
}

void lcd_set_data_pins_input(){
	PULLUP_OFF(LCD_DB4);
	PULLUP_OFF(LCD_DB5);
	PULLUP_OFF(LCD_DB6);
	PULLUP_OFF(LCD_DB7);
	_delay_us(1);
}

char lcd_read_nibble(){
	char tmp;
	SET(LCD_E);
	_delay_us(1);
	tmp = (READ(LCD_DB7)<<3)	
		| (READ(LCD_DB6)<<2)
		| (READ(LCD_DB5)<<1)
		| (READ(LCD_DB4));
	CLEAR(LCD_E);
	return(tmp);
}

void lcd_busy_wait_4_bit(){
	char tmp;
	tmp=0x80;
	CLEAR(LCD_RS);
	SET(LCD_RW);
	lcd_set_data_pins_input();
	while(tmp&0x80){
		tmp=lcd_read_nibble()<<4;
		tmp|=lcd_read_nibble();
	}
	_delay_us(4); // wait t_add time
	DDRAM_address=lcd_read_nibble()<<4;
	DDRAM_address|=lcd_read_nibble();
}
// lcd_set_type_data()
static inline void lcd_set_type_data(){
	SET(LCD_RS);
}

char lcd_read_byte(){
	SET(LCD_RS);
	SET(LCD_RW);
	char tmp;
	lcd_set_data_pins_input();
	tmp=lcd_read_nibble()<<4;
	tmp|=lcd_read_nibble();
	lcd_busy_wait_4_bit();
	return(tmp);
}

// lcd_set_type_command()
static inline void lcd_set_type_command(){
	CLEAR(LCD_RS);
}

// lcd_write_nibble(...)
void lcd_write_nibble(char data){
	CLEAR(LCD_RW);
	lcd_set_data_pins_output();
	if(data & 0x01) SET(LCD_DB4); else 	CLEAR(LCD_DB4);
	if(data & 0x02) SET(LCD_DB5); else	CLEAR(LCD_DB5);
	if(data & 0x04) SET(LCD_DB6); else 	CLEAR(LCD_DB6);
	if(data & 0x08) SET(LCD_DB7); else 	CLEAR(LCD_DB7);
	SET(LCD_E);
	_delay_us(1);
	CLEAR(LCD_E);
	_delay_us(1);
}

void lcd_write_byte(char c){
	lcd_write_nibble( (c >> 4) & 0x0f );
	lcd_write_nibble( c & 0x0f );
	lcd_busy_wait_4_bit();
}

void lcd_clear_and_home(){
	lcd_write_command(0x01);
	lcd_write_command(0x02);
}

void lcd_write_command(char c){
	lcd_set_type_command();
	lcd_write_byte(c);
}

void lcd_scroll_up(void){
	// scroll up
	char save_char;
	int ic=0;
	int ir=1;
	for(ic=0;ic<lcd_chars;ic++){
		for(ir=1;ir<lcd_lines;ir++){
			lcd_goto_position(ir,ic);
			lcd_set_type_data();
			save_char = lcd_read_byte();
			lcd_goto_position(ir-1,ic);
			lcd_write_data(save_char);
		}
	}
	lcd_goto_position(lcd_lines-1,0);
	for(ic=0;ic<lcd_chars;ic++){
		lcd_set_type_data();
		lcd_write_byte(' ');
	}
	lcd_goto_position(lcd_lines-1,0);
}

void lcd_write_data(char c){
	if((c==0x0D)|(c==0x0A)){					// process lf or cr
		if(DDRAM_address < lcd_chars){
			DDRAM_address = lcd_chars;
		}	else {
			if(DDRAM_address < LCD_OFFSET){
				DDRAM_address = LCD_OFFSET;
			}	else {
				if(DDRAM_address < LCD_OFFSET+lcd_chars){
					DDRAM_address = LCD_OFFSET+lcd_chars;
				}	else {
					DDRAM_address = 0;
				}
			}
		}
	} else {
		lcd_set_type_data();
		lcd_write_byte(c);
	}
	if(DDRAM_address == lcd_chars){
		lcd_goto_position(1,0);
	} else {
		if(DDRAM_address == LCD_OFFSET+lcd_chars){
			if(lcd_lines == 4){
				lcd_goto_position(2,0);
			} else {
				lcd_scroll_up();
			}
		} else {
			if(DDRAM_address == LCD_OFFSET){
				lcd_goto_position(3,0);
			} else {
				if(DDRAM_address == 0){
					lcd_scroll_up();
				}
			}
		}
	}
}

void 	lcd_cursor_blink(bool state){
	if(state){
		lcd_write_command(0x0D);	// turn on blinking cursor
	}else{
		lcd_write_command(0x0C);	// turn it off
	}
}

void lcd_goto_position(uint8_t row, uint8_t col){
	// 20x4 LCD: offsets 0, 0x40, 20, 0x40+20
	uint8_t row_offset = 0;
	if(row == 0){
			row_offset = 0;
	} else {
		if(row == 1){
				row_offset = LCD_OFFSET;
		} else {
			if(row == 2){
					row_offset = lcd_chars;
			} else {
				if(row == 3){
						row_offset = LCD_OFFSET+lcd_chars;
				}
			}
		}
	}
	lcd_write_command(0x80 | (row_offset + col));
}

int lcd_putchar(char c, FILE *stream){
	lcd_write_data(c);
	return 0;
}

void lcd_set_pattern(uint8_t index, uint8_t line1, uint8_t line2, uint8_t line3, uint8_t line4, uint8_t line5, uint8_t line6, uint8_t line7, uint8_t line8){
	uint8_t save_DDRAM_address = DDRAM_address;
	uint8_t set_GCRAM_addr = 0x40 | index<<3;
	CLEAR(LCD_RS);
	lcd_write_nibble((set_GCRAM_addr >> 4) & 0x0f );
	lcd_write_nibble(set_GCRAM_addr & 0x0f );
	_delay_us(40);
	SET(LCD_RS);
	lcd_write_nibble( (line1 >> 4) & 0x0f );
	lcd_write_nibble( line1 & 0x0f );
	_delay_us(40);
	lcd_write_nibble( (line2 >> 4) & 0x0f );
	lcd_write_nibble( line2 & 0x0f );
	_delay_us(40);
	lcd_write_nibble( (line3 >> 4) & 0x0f );
	lcd_write_nibble( line3 & 0x0f );
	_delay_us(40);
	lcd_write_nibble( (line4 >> 4) & 0x0f );
	lcd_write_nibble( line4 & 0x0f );
	_delay_us(40);
	lcd_write_nibble( (line5 >> 4) & 0x0f );
	lcd_write_nibble( line5 & 0x0f );
	_delay_us(40);
	lcd_write_nibble( (line6 >> 4) & 0x0f );
	lcd_write_nibble( line6 & 0x0f );
	_delay_us(40);
	lcd_write_nibble( (line7 >> 4) & 0x0f );
	lcd_write_nibble( line7 & 0x0f );
	_delay_us(40);
	lcd_write_nibble( (line8 >> 4) & 0x0f );
	lcd_write_nibble( line8 & 0x0f );
	_delay_us(40);

	lcd_write_command(0x80 | (save_DDRAM_address));
}

void lcd_init(uint8_t lines, uint8_t chars){
	lcd_lines = lines;
	lcd_chars = chars;
	// set pin driver directions
	OUTPUT(LCD_RS);		CLEAR(LCD_RS);
	OUTPUT(LCD_E);		CLEAR(LCD_E);
	OUTPUT(LCD_RW); 	CLEAR(LCD_RW);
	// wait 100msec
	_delay_ms(100);
	lcd_set_type_command();

	// do reset
	lcd_write_nibble(0x03);
	_delay_ms(6);
	lcd_write_nibble(0x03);
	_delay_us(150);
	lcd_write_nibble(0x03);
	_delay_us(150);
  
	// write 0010 (data length 4 bits)
	lcd_write_nibble(0x02);
	_delay_us(41);
	// set to 2 lines, font 5x8
	lcd_write_byte(0x28);
	// enable LCD
	lcd_write_byte(0x0c);
	// set entry mode
	lcd_write_byte(0x06);

	// set cursor/display shift
	lcd_write_byte(0x14);

	// clear and home
	lcd_clear_and_home();
}

#endif
