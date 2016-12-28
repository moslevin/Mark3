#!/bin/sh

# Build documentation source stage using the kernel and services directories
root_dir=`pwd`
echo "Root_Dir" ${root_dir}
cd ./kernel
make source ROOT_DIR=${root_dir}/
cd ${root_dir}
cd ./libs
make source ROOT_DIR=${root_dir}/
cd ${root_dir}
cd ./docs
doxygen ./Doxyfile
cd ${root_dir}
cd ./docs/output/latex/
pdflatex refman
makeindex refman
pdflatex refman
makeindex refman
pdflatex refman
cd ${root_dir}
cp ./docs/output/latex/refman.pdf ./docs
cd ..

