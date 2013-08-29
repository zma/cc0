#!/bin/bash

# current dir
dir=$(dirname "$0")

# common env
# . $dir/../conf/set-env.sh

export l0_dir=`readlink -f "$dir/"`

username_host=$1
basedir="/l0/"


ssh $username_host "mkdir $basedir/bin -p; date > $basedir/cc0-dist-date.txt"

ssh $username_host "rm -f $basedir/cc0"

file="toolchain/build/tools/cc0/cc0"
scp $l0_dir/$file $username_host:$basedir/cc0

scp -r $l0_dir/../../l0sys/libi0 $username_host:$basedir/

echo "Distributed to $username_host is done."

