#
#	cmd to run this file "make -f tiny10_toggle.mak"
#
# to build the demo program
PROGRAM=tiny10_toggle

# compiler/program parameters
MCU=attiny10
F_CPU=1000000UL

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

$(PROGRAM).o: $(PROGRAM).c  tiny10_toggle.mak
	avr-gcc -g -Os -mmcu=$(MCU) -std=c99 \
		-I../ \
		-ffunction-sections \
		-mtiny-stack \
    $< -o $@ \
		-Wl,--relax,--gc-sections \
		-Wl,-Map=$@.map -MMD -MP -MF $@.d \
		-DF_CPU=$(F_CPU) -DMCU=$(MCU)

$(PROGRAM).ass: $(PROGRAM).o
	avr-objdump -S -d $< > $@
	
$(PROGRAM).hex: $(PROGRAM).o $(PROGRAM).ass
	avr-objcopy -j .text -j .data -O ihex $< $@
	avr-size -A $(PROGRAM).hex 

$(PROGRAM).pgm: $(PROGRAM).hex
	avrdude $(AVRDUDE_PARMS) \
		-U flash:w:$<:a \
		-U fuse:w:0X$(FUSE):m \
		-U lock:w:0X$(LOCK):m

clean: 
	-rm *.o *.hex *.ass *.map *~ *.d

fuse:
	avrdude $(AVRDUDE_PARMS) \
	-U fuse:w:0X$(FUSE):m
