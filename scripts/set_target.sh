#!/bin/sh


if [ $# -lt 3 ]; then
    echo "Usage: set_target.sh <ARCH> <VARIANT> <TOOLCHAIN>"
    echo " "
    echo "  Where:"
    echo "    ARCH = architecture (i.e. avr, msp430, cm0, cm4f)"
    echo "    VARIANT = device variant (i.e. atmega328p, samd20, generic)"
    echo "    TOOLCHAIN = compiler toolchain (i.e gcc)"
    return 
fi

ARCH_LOCAL=$1
VARIANT_LOCAL=$2
TOOLCHAIN_LOCAL=$3

export ARCH=${ARCH_LOCAL}
export VARIANT=${VARIANT_LOCAL}
export TOOLCHAIN=${TOOLCHAIN_LOCAL}
export ROOT_DIR=`pwd`
