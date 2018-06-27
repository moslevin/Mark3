#!/bin/bash

# Build documentation source stage using the kernel and services directories
root_dir=`pwd`
cd ./kernel/docs
doxygen ./Doxyfile
cd ${root_dir}
cd ./kernel/docs/output/latex/
pdflatex refman
makeindex refman
pdflatex refman
makeindex refman
pdflatex refman
cd ${root_dir}
cp ./kernel/docs/output/latex/refman.pdf ./kernel/docs
