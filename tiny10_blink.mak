#
#	cmd to run this file "make -f tiny10_blink.mak"
#
# to build the demo program
PROGRAM=tiny10_blink

# compiler/program parameters
MCU=attiny10
F_CPU=8000000UL

# avrdude parameters
PGMR_COM_PORT=/dev/ttyACM0
PGMR_COM_BAUD=115200
AVRDUDE_PARMS = -C avrdude.rs -c AVR_TPI -p $(MCU) -b $(PGMR_COM_BAUD) -P $(PGMR_COM_PORT) -u

#	chip code locked, reset pin disabled
FUSE = FE
LOCK = FC

all: $(PROGRAM).pgm
compile: $(PROGRAM).hex

.SECONDARY:

%.o: %.c  tiny10_blink.mak
	avr-gcc -g -Os -mmcu=$(MCU) -std=c99 \
		-I../ \
		-ffunction-sections \
		-mtiny-stack \
    $< -o $@ \
		-Wl,--relax,--gc-sections \
		-Wl,-Map=$@.map -MMD -MP -MF $@.d \
		-DF_CPU=$(F_CPU) -DMCU=$(MCU)

%.ass: %.o
	avr-objdump -S -d $< > $@
	
%.hex: %.o %.ass
	avr-objcopy -j .text -j .data -O ihex $< $@
	avr-size -A $(*F).hex 

%.pgm: %.hex
	avrdude $(AVRDUDE_PARMS) \
		-U flash:w:$<:a \
		-U fuse:w:0X$(FUSE):m \
		-U lock:w:0X$(LOCK):m

clean: 
	-rm *.o *.hex *.ass *.map *~ *.d

fuse:
	avrdude $(AVRDUDE_PARMS) \
	-U fuse:w:0X$(FUSE):m
