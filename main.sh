#!/bin/sh


run () {
	lossMod=$1
	stepwidth=$2
	duration=$3
	packetsPerStep=$4
	startPos=$5
	tnr=$6
	rawVersion=$7
	./waf --run "scratch/practicalAssignment --lossMod=$lossMod --stepWidth=$stepwidth --duration=$duration --packetsPerStep=$packetsPerStep --startPos=$startPos" > helper/log_${tnr}_$lossMod 2>&1 && bash raw2csv_$rawVersion.sh helper/log_${tnr}_$lossMod
}

rm -rf helper
rm -rf plots
mkdir helper
mkdir plots

# 2.1
run 'f' '1' '351' '1' '0' '2_1' '2.1_3.1_4'
run 't' '1' '351' '1' '0' '2_1' '2.1_3.1_4'
run 'l' '1' '351' '1' '0' '2_1' '2.1_3.1_4'

# 2.2
run 'j' '0' '3601' '1' '10' '2_2' '2.2'

# 3.1
run 'c' '1' '351' '1' '0' '3_1' '2.1_3.1_4'

#3.2
run 'c' '5' '7100' '100' '0' '3_2' '3.2'

# 4
run 's' '1' '351' '1' '0' '4' '2.1_3.1_4'

gnuplot plot.plt
gnuplot plot22.plt
gnuplot plot32.plt
