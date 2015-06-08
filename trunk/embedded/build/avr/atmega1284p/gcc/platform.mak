# Platform-specific options

CC=avr-gcc
CPP=avr-gcc

CFLAGS=-funsigned-char -funsigned-bitfields -Os -fpack-struct -fshort-enums -g2 -Wall -c  -mmcu=$(VARIANT) -DAVR -DK_ADDR=K_USHORT -DK_WORD=uint8_t
CPPFLAGS=-funsigned-char -funsigned-bitfields -Os -fpack-struct -ffunction-sections -fshort-enums -g2 -Wall -c -mmcu=$(VARIANT) -DAVR -DK_ADDR=K_USHORT -DK_WORD=uint8_t

LINK=avr-gcc
LFLAGS= -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections  -mmcu=$(VARIANT)

AR=avr-ar
ARFLAGS=rcs

OBJCOPY=avr-objcopy
OBJCOPY_FLAGS=-O ihex -R .eeprom -R .fuse -R .lock -R .signature

CLANG=true
CLANGFLAGS=-D __AVR_ATmega1284P__ --analyze -fdiagnostics-show-category=name -Weverything 
