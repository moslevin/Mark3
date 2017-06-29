#!/bin/bash

ARCH="avr"
VARIANT="atmega328p"
TOOLCHAIN="gcc"

INCLUDES="
./kernel/public
./kernel/cpu/${ARCH}/${VARIANT}/${TOOLCHAIN}/public
./libs/dcpu/public
./libs/graphics/public
./libs/gui/public
./libs/heap/public
./libs/mark3c/public
./libs/memutil/public
./libs/nlfs/public
./libs/powerman/public
./libs/process/public
./libs/random/public
./libs/rtc/public
./libs/slip/public
./libs/slipterm/public
./libs/socket/public
./libs/streamer/public
./libs/sw_i2c/public
./libs/sw_spi/public
./libs/terminal/public
"

TIDY_INCLUDE_ARG=""
for INCLUDE in ${INCLUDES}; do
    TIDY_INCLUDE_ARG="${TIDY_INCLUDE_ARG} -I ${INCLUDE}"
done

KERNEL_SOURCES=$(find ./kernel -maxdepth 1 -type f -name "*.c" -or -name "*.cpp")
LIB_SOURCES=$(find ./libs -type f -name "*.cpp")

SOURCES="${KERNEL_SOURCES} ${PORT_SOURCES} ${LIB_SOURCES}"

CHECKS="cert-*,-cert-err58-cpp,clang-analyzer-*,hicpp-*,misc-*,performance-*,readability-*,-readability-redundant-declaration,-clang-diagnostic-*"

echo ${SOURCES}

for SOURCE in ${SOURCES}; do
    clang-tidy ${SOURCE} -checks=${CHECKS} -fix-errors -style=file -- ${TIDY_INCLUDE_ARG} -I "C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\avr\include" -D __AVR_ATmega38P__ -D AVR
done

