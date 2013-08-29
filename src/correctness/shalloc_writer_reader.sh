#!/bin/bash

# the program src code
program=shalloc_writer_reader.c

amout="/tmp/am.out.`whoami`"

cc0 $program -o $program.bin > /dev/null || echo "ERROR" 
echo "OK"

