#!/bin/bash

dir=$(dirname "$0")

mkdir -p $dir/toolchain/build/

cd $dir/toolchain/build/

git pull # | grep 'up-to-date'

cmake ../
make

cd -

