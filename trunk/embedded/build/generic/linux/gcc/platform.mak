# Platform-specific options

CC=gcc
CPP=g++

CFLAGS=-g2 -DK_ADDR=uint64_t -c
CPPFLAGS=-g2 -DK_ADDR=uint64_t -c

LINK=avr-gcc
LFLAGS=-lrt -lpthread

AR=ar
ARFLAGS=rcs

OBJCOPY=objcopy
OBJCOPY_FLAGS=-O ihex

CLANG=clang
CLANGFLAGS=--analyze -fdiagnostics-show-category=name -Weverything  

