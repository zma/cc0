#!/bin/bash

program=ppm.c
amout="/tmp/am.out.`whoami`"

# echo -n "[~15 min] "

for ii in r w i
do
    cc0 ppm-$ii.c -o ppm-$ii.bin > /dev/null || echo "ERROR" 
echo "OK"

done


