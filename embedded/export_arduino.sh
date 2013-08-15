#!/bin/bash
#-- 
# Utility script to export kernel for atmega328p as an arduino library

## Purge the output folders
if [ -d "./arduino/Mark3" ]; then
	rmdir ./arduino/Mark3
fi
if [ -d "./arduino/output" ]; then
	rmdir ./arduino/output
fi

rm ./arduino/*.cpp
rm ./arduino/*.h
rm ./arduino/*.zip

## Copy kernel source and headers
cp -f ./kernel/*.cpp ./arduino
cp -f ./kernel/public/*.h ./arduino
cp -f ./kernel/cpu/avr/atmega328p/gcc/*.cpp ./arduino
cp -f ./kernel/cpu/avr/atmega328p/gcc/public/*.h ./arduino
cp -f ./arduino/mark3cfg.h_arduino ./arduino/mark3cfg.h


## Build doxygen docs (assumes pdflatex and doxygen are installed)
cd ./arduino
doxygen ./Doxyfile_arduino
CURR_DIR=`pwd`
cd ./output/latex
pdflatex refman
makeindex refman
pdflatex refman
makeindex refman
pdflatex refman
cd ${CURR_DIR}

## Copy the source and generated docs out to a "Mark3" subdir
mkdir ./Mark3
mv -f ./*.cpp ./Mark3
mv -f ./*.h ./Mark3
cp -f ./*.txt ./Mark3
mkdir ./Mark3/Docs
cp -f ./output/latex/refman.pdf ./Mark3/Docs
cp -fR ./output/html ./Mark3/Docs

## Create an archive containing the exported source and docs
zip -r Mark3.zip Mark3

