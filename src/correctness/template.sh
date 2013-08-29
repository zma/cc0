#!/bin/bash

# EDIT here: the program src code
program=double-division.c

echo -n "$program "

amout="/tmp/am.out.`whoami`"

cc0 $program -o $program.bin > /dev/null

# am slay k > /dev/null

am run $program.bin > $amout

# the 1st line in STDOUT
line1=`cat $amout | grep -A10 "The STDOUTs" | head -n3 | tail -n1`
# the 2nd line in STDOUT
line2=`cat $amout | grep -A10 "The STDOUTs" | head -n4 | tail -n1`

# EDIT here: test correctness condition
if [ "$line1" == "4602678819172646912" ]
then
    echo "PASSED"
    exit 0
else
    echo "FAIL"
    exit 1
fi

