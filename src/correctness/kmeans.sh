#!/bin/bash

wget http://baijia.info/l0/kmeans.l0.v2.tar.bz2 > /dev/null 2>&1
tar xf kmeans.l0.v2.tar.bz2 
rm -rf kmeans.l0.v2.tar.bz2
cd kmeans

make > /dev/null || echo "ERROR" 
echo "OK"

