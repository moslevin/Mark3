#!/bin/bash

RETURN_DIR=$(pwd)

cd ./out/${M3_ARCH}_${M3_VARIANT}_${M3_TOOLCHAIN}
ninja
cd ${RETURN_DIR}
