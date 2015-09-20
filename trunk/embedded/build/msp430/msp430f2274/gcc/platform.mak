# Platform-specific options

CC=msp430-gcc
CPP=msp430-g++

CFLAGS=-funsigned-char -funsigned-bitfields -O0 -fpack-struct  -fshort-enums -Wall -c -mmcu=$(VARIANT) -DK_ADDR=uint16_t -DK_WORD=uint16_t
CPPFLAGS=-funsigned-char -funsigned-bitfields -O0 -fpack-struct -fshort-enums -Wall -c -mmcu=$(VARIANT) -DK_ADDR=uint16_t -DK_WORD=uint16_t

LINK=msp430-gcc
LFLAGS=-Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=$(VARIANT)

AR=msp430-ar
ARFLAGS=rcs

OBJCOPY=msp430-objcopy
OBJCOPY_FLAGS=-O ihex

CLANG=true
CLANGFLAGS= --analyze -fdiagnostics-show-category=name -Weverything

CFLAGS+=-I/usr/msp430/include/
CPPFLAGS+=-I/usr/msp430/include/
CLANGFLAGS+=-I/usr/msp430/include/
