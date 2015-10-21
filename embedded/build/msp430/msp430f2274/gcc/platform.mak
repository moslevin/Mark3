# Platform-specific options

CC=msp430-gcc
CPP=msp430-g++

CFLAGS=-funsigned-char -funsigned-bitfields -Os -g3 -fpack-struct  -fshort-enums -Wall -c -mmcu=$(VARIANT) -DK_ADDR=uint16_t -DK_WORD=uint16_t
CPPFLAGS=-funsigned-char -funsigned-bitfields -Os -g3 -fpack-struct -fshort-enums -Wall -c -mmcu=$(VARIANT) -DK_ADDR=uint16_t -DK_WORD=uint16_t

LINK=msp430-gcc
LFLAGS=-Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=$(VARIANT)
LFLAGS_DBG= -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--section-start=.logger=0x1000000 -mmcu=$(VARIANT) -I~/atmel/avr/include

AR=msp430-ar
ARFLAGS=rcs

OBJCOPY=msp430-objcopy
OBJCOPY_FLAGS=-O ihex
OBJCOPY_DBG_FLAGS=--only-section=.logger -O binary --set-section-flags .logger=alloc --change-section-address .logger=0

CFLAGS+=-I/usr/msp430/include/
CPPFLAGS+=-I/usr/msp430/include/
