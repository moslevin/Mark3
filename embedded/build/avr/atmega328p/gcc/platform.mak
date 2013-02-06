# Platform-specific options

CC=avr-gcc
CPP=avr-gcc

CFLAGS=-funsigned-char -funsigned-bitfields -Os -fpack-struct -fshort-enums -g2 -Wall -pedantic -c -mmcu=$(VARIANT) -I~/atmel/avr/include -DAVR
CPPFLAGS=-funsigned-char -funsigned-bitfields -Os -fpack-struct -ffunction-sections -fshort-enums -g2 -Wall -pedantic -c -mmcu=$(VARIANT) -I~/atmel/avr/include -DAVR

LINK=avr-gcc
LFLAGS= -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections  -mmcu=$(VARIANT) -I~/atmel/avr/include

AR=avr-ar
ARFLAGS=rcs

OBJCOPY=avr-objcopy
OBJCOPY_FLAGS=-O ihex -R .eeprom -R .fuse -R .lock -R .signature

CLANG=asdf
CLANGFLAGS=-D __AVR_ATmega328P__ --analyze -fdiagnostics-show-category=name -Weverything  

