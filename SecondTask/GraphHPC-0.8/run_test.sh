#!/bin/bash
make

dir="runDir"
in="rmat-$1"
out1="$in.1"
out2="$in.2"

rm -rf $dir
mkdir $dir
cd $dir

../gen_RMAT_serial -s $1
../graphs_reference -sssp -in $in -out $out1
../graphs_reference_fb -sssp -in $in -out $out2
../compare $out1 $out2
