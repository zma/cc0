#!/bin/bash

dir=$(pwd)

sudo yum -y groupinstall 'Development tools'
sudo yum -y install cmake glibc-static libstdc++-static boost boost-serialization boost-devel gcc-c++ tar wget

if [ $? -eq 0 ]; then
	if [ ! -f ./libantlr3c-3.4.tar.gz ]; then
		wget http://www.antlr3.org/download/C/libantlr3c-3.4.tar.gz
		tar -xzvf ./libantlr3c-3.4.tar.gz
		cd libantlr3c-3.4
		./configure --enable-64bit
		make
		sudo make install
	fi
fi

if [ $? -eq 0 ]; then
	cd $dir
	./build-cc0.sh
fi
