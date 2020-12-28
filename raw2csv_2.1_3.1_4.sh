#!/bin/sh
cat $1 | grep "[0-9]\+$" > $1.csv
