#!/bin/bash

dir=$(dirname "$0")

mkdir -p $dir/toolchain/build/

cd $dir/toolchain/build/

cmake ../
make

cd -

