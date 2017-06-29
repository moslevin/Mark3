#!/bin/bash

TEST_STAMP=$(date +%Y%m%d_%H%M)
LOG_FILE=$(pwd)/${TEST_STAMP}_build_log.txt

cd /media/usb/project/github/Mark3 
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

TARGET_LIST="arduino arduino2560"

for TARGET in ${TARGET_LIST}; do
    echo "===[Exporting SDK for ${TARGET}]==="
    echo "===[Exporting SDK for ${TARGET}]===" >>${LOG_FILE} 
    bash -c ". ./scripts/export.sh ${TARGET}" 2>&1 >>${LOG_FILE} 
done 

