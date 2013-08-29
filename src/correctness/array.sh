#!/bin/bash

program=array.c
amout="/tmp/am.out.`whoami`"

cc0 $program -o $program.bin || echo "ERROR" 
echo "OK"


