#!/bin/sh
cat "$1" | grep "^+" | sed 's/s propagation:.*rxPower=/,/' | sed 's/dbm.*//' | sed 's/\([^,]*\),\([0-9.\-]*\)/\2,\1/' > "$1".csv
