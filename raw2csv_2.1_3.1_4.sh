#!/bin/sh
cat $1 | grep "^+" | sed 's/.*rxPower=//' | sed 's/dbm, distance=/,/' | sed 's/m,.*//' > $1.csv
