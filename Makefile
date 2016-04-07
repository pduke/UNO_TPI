# build the AVR_TPI Programmer
PROGRAM = UNO_TPI

# compiler/program parameters
MCU = atmega328p
F_CPU = 16000000
BAUD = 115200
LCD_4BIT = YES			# must be exactly YES or NO
GCC_PARAMS = -DLCD_4BIT=$(LCD_4BIT) -DF_CPU=$(F_CPU) -DBAUD=$(BAUD)

# avrdude parameters
PROGRAMMER = "arduino"
PGMR_COM_PORT = /dev/ttyACM0
PGMR_COM_BAUD = 115200
AVRDUDE_PARAMS = -c $(PROGRAMMER) -p $(MCU) -b $(PGMR_COM_BAUD) -P $(PGMR_COM_PORT)

all: $(PROGRAM).hex				# 'make' will compile only

flash: $(PROGRAM).pgm			# 'make flash' will compile and upload to Arduino

.SECONDARY:

$(PROGRAM).o: $(PROGRAM).c Makefile ../libNoter/lcd_Noter_4_bit.h
	avr-gcc -g -Os -fno-jump-tables -Wall -mmcu=$(MCU) -mcall-prologues \
		-I ../ -std=c99 \
		-Wl,-u,vfprintf -lprintf_flt -Wl,-u,vfscanf -lscanf_flt -lm \
		$< -o $@ \
		$(GCC_PARAMS)
	
$(PROGRAM).ass: $(PROGRAM).o
	avr-objdump -S -d $< > $@
	
$(PROGRAM).hex: $(PROGRAM).o $(PROGRAM).ass
	avr-objcopy -j .text -O ihex $< $@
	avr-size -A $(PROGRAM).hex 

$(PROGRAM).pgm: $(PROGRAM).hex
	avrdude $(AVRDUDE_PARAMS) \
		-U flash:w:$<:a 


clean: 
	-rm *.o *.hex *.ass *.map *.o.d *~
