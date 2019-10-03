################################################################################
# Makefile for Egret LightCtrl                                                 #
# ---------------------------------------------------------------------------- #
# author:  Markus Lohse <mlohse@gmx.net>                                       #
# date:    2019-09-19                                                          #
# license: LGPLv3, see https://www.gnu.org/licenses/lgpl-3.0.txt               #
################################################################################

# Target name:
TARGET = lightctrl

# Fuses
HFUSE=0x99
LFUSE=0xEB

# Target type
MCU=atmega1284p
PROGRAMMER=dragon_isp

SRC =	main.c \
		uart.c \
		timer.c \
		buffer.c \
		sleep.c \
		msg.c \
		ctrl.c \
		led.c \
		adc.c

OBJ = $(SRC:.c=.o)


# Compiler / Linker flags:
CFLAGS = -mmcu=$(MCU) -Wall -g2 -O1 -std=gnu99 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
LDFLAGS = -mmcu=$(MCU) -Wl,-Map=$(TARGET).map

###################################
# rules:

.PHONY: all program clean

all:	$(TARGET).hex $(TARGET).eep $(TARGET).lss

fuses:
	avrdude -c $(PROGRAMMER) -P usb -p $(MCU) -U hfuse:w:$(HFUSE):m -U lfuse:w:$(LFUSE):m

program:
	avrdude -c $(PROGRAMMER) -P usb -p $(MCU) -U flash:w:$(TARGET).hex
	#sleep 2
	#avrdude -c $(PROGRAMMER) -P usb -p $(MCU) -U eeprom:w:$(TARGET).eep

clean:
	rm -f $(OBJ) $(TARGET).elf $(TARGET).hex $(TARGET).lss $(TARGET).map $(TARGET).eep


###################################
# Psudorules:

%.eep: %.elf
	avr-objcopy -j .eeprom --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 --no-change-warnings -O ihex $< $@

%.hex: %.elf
	avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature $< $@

%.lss: %.elf
	avr-objdump -h -S $< > $@

%.elf: $(OBJ)
	avr-gcc $^ $(LDFLAGS) -o $@
	avr-size $@

%.o : %.c
	avr-gcc $(CFLAGS) -c $< -o $@
