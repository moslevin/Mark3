# Platform-specific options

CC=avr-gcc
CPP=avr-gcc

CFLAGS=-funsigned-char -funsigned-bitfields -Os -g3 -fpack-struct -fshort-enums -Wall -Wno-unused-variable -c -mmcu=$(VARIANT) -I~/atmel/avr/include -DAVR -DK_ADDR=uint16_t -DK_WORD=uint8_t
CPPFLAGS=-funsigned-char -funsigned-bitfields -Os -g3 -fpack-struct -ffunction-sections -fshort-enums -Wall -Wno-unused-variable -c -mmcu=$(VARIANT) -I~/atmel/avr/include -DAVR -DK_ADDR=uint16_t -DK_WORD=uint8_t

LINK=avr-gcc
LFLAGS= -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=$(VARIANT) -I~/atmel/avr/include
LFLAGS_DBG= -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--section-start=.logger=0x1000000 -mmcu=$(VARIANT) -I~/atmel/avr/include

AR=avr-ar
ARFLAGS=rcs

OBJCOPY=avr-objcopy
OBJCOPY_FLAGS=-O ihex -R .eeprom -R .fuse -R .lock -R .signature
OBJCOPY_DBG_FLAGS=--only-section=.logger -O binary --set-section-flags .logger=alloc --change-section-address .logger=0

CLANG=true
CLANGFLAGS=-D __AVR_ATmega328P__ --analyze -fdiagnostics-show-category=name -Weverything  

CFLAGS+=-I/usr/lib/avr/include/
CPPFLAGS+=-I/usr/lib/avr/include/
CLANGFLAGS+=-I/usr/lib/avr/include/
