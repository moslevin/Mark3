#!/bin/bash

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

export ROOT_DIR=$(pwd)
if [ -d "./kbuild" ]; then
    rm -r ./kbuild
fi

mkdir ./kbuild
if [ -d "./kbuild" ]; then
    cd ./kbuild
    cmake .. -DCMAKE_TOOLCHAIN_FILE=../mark3.cmake -Dmark3_arch=${ARCH_LOCAL} -Dmark3_variant=${VARIANT_LOCAL} -Dmark3_toolchain=${TOOLCHAIN_LOCAL} -Dmark3_root_dir=${ROOT_DIR} -GNinja
    cd ..
fi
