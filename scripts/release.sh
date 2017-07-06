#!/bin/bash

TEST_STAMP=$(date +%Y%m%d_%H%M)
LOG_FILE="${TEST_STAMP}_build_log.txt"

echo "===[Git config]==="
echo "===[Git config]===" >> ${LOG_FILE}
git stash 2>&1 >>${LOG_FILE}
git checkout master 2>&1 >>${LOG_FILE}
git pull 2>&1 >>${LOG_FILE}
git checkout -b release_tag_${TEST_STAMP} 2>&1 >>${LOG_FILE}

echo "===[Build config]==="
echo "===[Build config]===" >> ${LOG_FILE}
bash -c ". ./scripts/set_target.sh avr atmega328p gcc" 2>&1 >>${LOG_FILE}
echo "===[Build]==="
echo "===[Build]===" >> ${LOG_FILE}
bash -c ". ./scripts/build.sh" 2>&1 >>${LOG_FILE} 
echo "===[Unit Testing]==="
echo "===[Unit Testing]===" >> ${LOG_FILE}
bash -c "python ./scripts/sim_test.py" 2>&1 >>${LOG_FILE} 
echo "===[Runtime profiling (doc)]==="
echo "===[Runtime profiling (doc)]===" >> ${LOG_FILE}
bash -c ". ./scripts/profile_sim.sh" 2>&1 >>${LOG_FILE} 
echo "===[Build size profiling (doc)]==="
echo "===[Build size profiling (doc)]===" >> ${LOG_FILE}
bash -c ". ./scripts/size_profile.sh avr atm3ga328p gcc -d" > ./kernel/public/sizeprofile.h
echo "===[Doxygen build (doc)]==="
echo "===[Doxygen build (doc)]===" >> ${LOG_FILE}
bash -c ". ./scripts/build_docs.sh" 2>&1 >>${LOG_FILE} 

rm -r ./export
mkdir ./export
TARGET_LIST="arduino arduino2560"
for TARGET in ${TARGET_LIST}; do
    echo "===[Exporting SDK for ${TARGET}]==="
    echo "===[Exporting SDK for ${TARGET}]===" >>${LOG_FILE} 
    bash -c ". ./scripts/export.sh ${TARGET}" 2>&1 >>${LOG_FILE} 
done 

echo "===[Exporting Source Distribution]==="
SRC_EXPORT=./export/distro
mkdir ${SRC_EXPORT}
FOLDERS="apps arduino bootloader build drivers examples fonts kernel libs scripts tests"
for FOLDER in ${FOLDERS}; do
     cp -R ${FOLDER} ${SRC_EXPORT}
done
TXTFILES="readme.txt mark3.cmake license.txt"
for TXTFILE in ${TXTFILES}; do
     cp ${TXTFILE} ${SRC_EXPORT}
done

mkdir ${SRC_EXPORT}/util
cp ./util/*.exe ${SRC_EXPORT}/util

mkdir ${SRC_EXPORT}/docs
cp ./docs/refman.pdf ${SRC_EXPORT}/docs

BUILD_DATE=`date +%C%y%m%d`
ZIPNAME="Mark3_${BUILD_DATE}.zip"
cd ./export
zip -r ${ZIPNAME} distro
cd ..
