#!/bin/bash

dir=$(dirname "$0")

mkdir -p $dir/toolchain/build/

cd $dir/toolchain/build/

# Accept an optional command line parameter to set the build type.
# Valid build types are Debug, Release, RelWithDebInfo, MinSizeRel
if [ -z "$1" ]; then
	cmake -DCMAKE_BUILD_TYPE=$1 -DCC0_USE_STATIC_CRT=1 ..
else
	cmake -DCC0_USE_STATIC_CRT=1
fi

# make VERBOSE=1
make

cd -

