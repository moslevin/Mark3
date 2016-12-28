#!/bin/bash

ELF_FILES=`find ./stage/dbg -type f -and -name *.elf`

# Go through all of the elf files in the pathspace and extract out all of the debug strings into raw binary format, stored
# as raw binary files that are easily processed by a log reassembler/viewer
for ELF_FILE in ${ELF_FILES}; do
    DEST_FILE=`dirname ${ELF_FILE}`
    ELF_NAME=`basename ${ELF_FILE}`
    DEST_FILE="${DEST_FILE}/${ELF_NAME}.bin"
    echo "SRC: ${ELF_FILE}"
    echo "DST: ${DEST_FILE}"
    avr-objcopy --only-section=.logger -O binary --set-section-flags .logger=alloc ${ELF_FILE} ${DEST_FILE}
done
