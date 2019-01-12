#!/bin/bash
#-- 
# Utility script to export the kernel for various platforms.


ARCH=""
VARIANT=""
TOOLCHAIN=""
SHOW_USAGE=0
TARGET_FOUND=0
CURR_DIR=$(pwd)

### Validate Argument Count
if [ $# -lt 3 ]; then
	SHOW_USAGE=1
else
	ARCH=$1
	VARIANT=$2
	TOOLCHAIN=$3
fi

### Validate chosen target
if [ ${SHOW_USAGE} -eq 0 ]; then
	if [ -d "${CURR_DIR}/kernel/src/arch/${ARCH}/${VARIANT}/${TOOLCHAIN}" ]; then
		TARGET_FOUND=1
		echo "[ Exporting kernel source for ${ARCH}/${VARIANT}/${TOOLCHAIN} ]"
	fi

	if [ ${TARGET_FOUND} -eq 0 ]; then
		SHOW_USAGE=1
	fi
fi

### Show the "usage" message if the parameters weren't entered correctly
if [ ${SHOW_USAGE} -eq 1 ]; then
	echo ""
	echo "----[Mark3 RTOS - (c) 2013-2019 m0slevin]----"
	echo "export.sh - Export Mark3 source for a specific platform"
	echo ""
	echo "Usage: "
	echo "    export.sh <architecture> <target> <toolchain>"
	echo ""
	return
fi

### Clean up/Recreate missing directories ###
ROOT_DIR=$(pwd)"/export/"
TARGET_DIR=$(pwd)"/export/${ARCH}_${VARIANT}_${TOOLCHAIN}"
TARGET="${ARCH}_${VARIANT}_${TOOLCHAIN}"

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
cp -f ./kernel/src/*.cpp ${TARGET_DIR}/Mark3/
cp -f ./kernel/src/public/*.h ${TARGET_DIR}/Mark3/
cp -f ./kernel/src/arch/${ARCH}/${VARIANT}/${TOOLCHAIN}/*.cpp ${TARGET_DIR}/Mark3/
cp -f ./kernel/src/arch/${ARCH}/${VARIANT}/${TOOLCHAIN}/public/*.h ${TARGET_DIR}/Mark3/

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
	mkdir ${TARGET_DIR}/Mark3/Doc
	cp ./build/doxyfile_export.in ${TARGET_DIR}/Mark3/

	mkdir ${TARGET_DIR}/Mark3/Doc/graphics
	cp ./kernel/doc/graphics/*.png ${TARGET_DIR}/Mark3/Doc/graphics/

	cd ${TARGET_DIR}/Mark3
	echo "[ Generating doxygen docs ]"
	doxygen ./doxyfile_export.in 2> /dev/null >> /dev/null 
	echo "[ Done generating doxygen docs ]"

	mv ./output/html ${TARGET_DIR}/Mark3/Doc/

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
BUILD_DATE=`date +%C%y%m%d`
ZIPNAME="Mark3_${TARGET}_${BUILD_DATE}.zip"
cd ${TARGET_DIR}
echo "[ Creating Source/Doc Archive ]"
zip -r ${ZIPNAME} Mark3 2> /dev/null >> /dev/null 

## Move the .zip to the "output" folder
mv ${ZIPNAME} ./Output

## CD back to the starting directory
cd ${CURR_DIR}

## We're done
echo "[ Finished! ]"


