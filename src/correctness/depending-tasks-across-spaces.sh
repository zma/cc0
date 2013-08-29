#!/bin/bash

# EDIT here: the program src code
program=depending-tasks-across-spaces.c

echo -n "$program "

amout="/tmp/am.out.`whoami`"

cc0 $program -o $program.bin > /dev/null || echo "ERROR" 
echo "OK"

