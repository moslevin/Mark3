#!/bin/bash

# Build documentation source stage using the kernel and services directories
root_dir=`pwd`
cd ./kernel/doc
doxygen ./Doxyfile_release.in
cd ${root_dir}
cd ./kernel/doc/output/latex/
pdflatex refman
makeindex refman
pdflatex refman
makeindex refman
pdflatex refman
cd ${root_dir}
cp ./kernel/doc/output/latex/refman.pdf ./kernel/doc
