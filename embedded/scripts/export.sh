#!/bin/bash
#-- 
# Utility script to export the kernel for various platforms.

### Supported targets for export
TARGET_LIST="atmega328p atmega644 atmega1280 atmega2560 atmega1284p atxmega256a3 arduino arduino2560 samd20 cortex_m0 cortex_m3 cortex_m4f msp430f2274"
TARGET=""
ARCH=""
VARIANT=""
TOOLCHAIN="gcc"
SHOW_USAGE=0
TARGET_FOUND=0
CURR_DIR=$(pwd)

### Validate Argument Count
if [ $# -lt 1 ]; then
	SHOW_USAGE=1
fi

### Validate chosen target
if [ ${SHOW_USAGE} -eq 0 ]; then
	for TARGET_ENTRY in ${TARGET_LIST}; do
		if [ "${TARGET_ENTRY}" == "$1" ]; then
			TARGET_FOUND=1
			TARGET=$1
			echo "[ Exporting kernel source for ${TARGET} ]"
			break
		fi
	done
	if [ ${TARGET_FOUND} -eq 0 ]; then
		SHOW_USAGE=1
	fi
fi

### Show the "usage" message if the parameters weren't entered correctly
if [ ${SHOW_USAGE} -eq 1 ]; then
	echo ""
	echo "----[Mark3 RTOS - (c) 2013 Funkenstein Software Consulting]----"
	echo "export.sh - Export Mark3 source for a specific platform"
	echo ""
	echo "Usage: "
	echo "    export.sh <target> [-d]"
	echo ""
	echo "Where target is one of the following"
	for TARGET_ENTRY in ${TARGET_LIST}; do
		echo "    ${TARGET_ENTRY}"
	done
	echo ""
	exit
fi

### Select architecture variable based on target
case ${TARGET} in
	"atmega"* | "atxmega"* | "arduino"* )
		echo "[ Target is an Atmel AVR part ]"	
		ARCH="avr"
		;;
	"samd20" | "cortex_m0")
		echo "[ Target is an ARM Cortex M0 part ]"
		ARCH="cm0"
		;;
	"cortex_m3" )
		echo "[ Target is an ARM Cortex M3 part ]"
		ARCH="cm3"
		;;
	"cortex_m4f" )
		echo "[ Target is an ARM Cortex M4F part ]"
		ARCH="cm4f"
		;;
	"msp430f2274")
		echo "[ Target is a TI MSP430 part ]"
		ARCH="msp430"
		;;
	*)
		echo "Error selecting target architecture, bailing"
		exit
		;;
esac

### Set the VARIANT variable based on target ###
case ${TARGET} in
	"atmega328p" | \
	"atmega644" | \
	"atmega1280" | \
	"atmega2560" | \
	"atmega1284p" | \
	"atxmega256a3" | \
	"msp430f2274" | \
	"samd20" )
		### Target names match variant names ###
		VARIANT=${TARGET}
		;;
	"arduino")
		### Arduino is a repackaged atmega328p, with some extra shtuff ###
		VARIANT="atmega328p"
		;;
	"arduino2560")
		### Arduino is a repackaged atmega2560, with some extra shtuff ###
		VARIANT="atmega2560"
		;;
	"cortex_m0")
		### Generic cortex_m0 is just the stm32f0 port ###
		VARIANT="generic"
		;;
	"cortex_m3")
		### Generic cortex_m3 is its own port ###
		VARIANT="generic"
		;;
	"cortex_m4f")
		### Generic cortex_m4f is its own port ###
		VARIANT="generic"
		;;
	*)
		echo "Error selecting target variant, bailing"
		exit
		;;
esac

### Clean up/Recreate missing directories ###
ROOT_DIR=$(pwd)"/export/"
TARGET_DIR=$(pwd)"/export/${TARGET}"

echo "[ Target Directory: ${TARGET_DIR} ]"

if [ ! -d ${ROOT_DIR} ]; then
	mkdir ${ROOT_DIR}
fi

if [ -d "${TARGET_DIR}" ]; then
	rm -r ${TARGET_DIR}/*
else
	mkdir ${TARGET_DIR}
fi

mkdir "${TARGET_DIR}/Mark3"
mkdir "${TARGET_DIR}/Output"

### Copy code into the new folder ###
cp -f ./kernel/*.cpp ${TARGET_DIR}/Mark3/
cp -f ./kernel/public/*.h ${TARGET_DIR}/Mark3/
cp -f ./kernel/cpu/${ARCH}/${VARIANT}/${TOOLCHAIN}/*.cpp ${TARGET_DIR}/Mark3/
cp -f ./kernel/cpu/${ARCH}/${VARIANT}/${TOOLCHAIN}/public/*.h ${TARGET_DIR}/Mark3/

### Target-specific bits... ###
case ${TARGET} in
	"arduino"* )
		mkdir ${TARGET_DIR}/Mark3/inc
		mv ${TARGET_DIR}/Mark3/*.h ${TARGET_DIR}/Mark3/inc
		cp -f ./arduino/mark3cfg.h_arduino ${TARGET_DIR}/Mark3/inc/mark3cfg.h
		cp -f ./arduino/kerneltypes.h ${TARGET_DIR}/Mark3/inc/kerneltypes.h
		cp -f ./arduino/mark3.h_arduino ${TARGET_DIR}/Mark3/mark3.h
		cp -f ./arduino/keywords.txt ${TARGET_DIR}/Mark3
		SOURCE_FILES=`ls -1 ${TARGET_DIR}/Mark3/*.cpp`
		for SOURCE_FILE in ${SOURCE_FILES}; do
			cat ${SOURCE_FILE} | sed -e 's/#include \"/#include \"\.\/inc\//' > tmp.cpp
			cp -f ./tmp.cpp ${SOURCE_FILE}
		done
		rm tmp.cpp
		;;
	"samd20" )
		cp -f ./drivers/cpu/cm0/samd20/gcc/system/*.cpp ${TARGET_DIR}/Mark3/
		cp -f ./drivers/cpu/cm0/samd20/gcc/system/public/*.h ${TARGET_DIR}/Mark3/
		;;
	*)
		;;
esac

## Build doxygen docs if doxygen/pdflatex are installed
IS_LATEX_INSTALLED=$(which pdflatex)
IS_DOXYGEN_INSTALLED=$(which doxygen)

USE_DOXYGEN=0
if [ "" != "${IS_DOXYGEN_INSTALLED}" ]; then
	USE_DOXYGEN=1
	echo "[ Doxygen found... will generate docs ]"
else
	echo "[ Doxygen not found... no documentation will be generated ]"
fi

USE_LATEX=0
if [ "" != "${IS_LATEX_INSTALLED}" ]; then
	USE_LATEX=1
	echo "[ Pdflatex found... will generate PDF from doxygen output ]"
else
	echo "[ Pdflatex not found... no PDF documentation will be generated ]"
fi

if [ ${USE_DOXYGEN} -eq 1 ]; then
	mkdir ${TARGET_DIR}/Mark3/Docs
	cp ./docs/doxyfile_export ${TARGET_DIR}/Mark3/

	mkdir ${TARGET_DIR}/Mark3/Docs/graphics
	cp ./docs/graphics/*.png ${TARGET_DIR}/Mark3/Docs/graphics/

	cd ${TARGET_DIR}/Mark3
	echo "[ Generating doxygen docs ]"
	doxygen ./doxyfile_export 2> /dev/null >> /dev/null 
	echo "[ Done generating doxygen docs ]"

	mv ./output/html ${TARGET_DIR}/Mark3/Docs/

	if [ ${USE_LATEX} -eq 1 ]; then
		cd ./output/latex/
		echo "[ Generating PDF docs ]"
		pdflatex refman 2> /dev/null >> /dev/null 
		makeindex refman 2> /dev/null >> /dev/null 
		pdflatex refman 2> /dev/null >> /dev/null 
		makeindex refman 2> /dev/null >> /dev/null 
		pdflatex refman	2> /dev/null >> /dev/null 
		echo "[ Done generating PDF docs ]"

		mv ./refman.pdf ${TARGET_DIR}/Mark3/Docs
	fi
	
	rm -r ${TARGET_DIR}/Mark3/output
	cd ${CURR_DIR}
	rm ${TARGET_DIR}/Mark3/doxyfile_export	
fi

## Create a .zip file tagged against the SVN revision and date
SVN_REV=`svn info | grep Revision | sed 's/Revision: //'`
BUILD_DATE=`date +%C%y%m%d`
ZIPNAME="Mark3_"${TARGET}"_r"${SVN_REV}"_"${BUILD_DATE}".zip"
cd ${TARGET_DIR}
echo "[ Creating Source/Doc Archive ]"
zip -r ${ZIPNAME} Mark3 2> /dev/null >> /dev/null 

## Move the .zip to the "output" folder
mv ${ZIPNAME} ./Output

## CD back to the starting directory
cd ${CURR_DIR}

## We're done

echo "[ Finished! ]"
exit

