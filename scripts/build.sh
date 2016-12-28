#!/bin/bash

# Shell script to build a clean Mark3 kernel, all demo and test apps, with
# full Doxygen documentation.
if [ "${ROOT_DIR}" == "" ]; then
    ROOT_DIR=$(pwd)
fi
echo "ROOT=${ROOT_DIR}"

export ROOT_DIR=${ROOT_DIR}

# Clean out development stage
rm -rf ./stage/inc/*
rm -rf ./stage/app/*
rm -rf ./stage/lib/*
rm -rf ./stage/drv/*
rm -rf ./stage/src/*
rm -rf ./stage/sa/*

# Purge object files for all components
make clean

# Build working set of headers, libraries, and applications (will not stop on errors!)
make headers
make library -j4
make binary

