#!/bin/bash

dir=$(dirname "$0")

git pull # | grep 'up-to-date'

$dir/build-cc0.sh

