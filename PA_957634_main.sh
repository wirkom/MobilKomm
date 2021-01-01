#!/bin/sh


run () {
	lossMod=$1
	stepwidth=$2
	duration=$3
	packetsPerStep=$4
	startPos=$5
	tnr=$6
	rawVersion=$7
	./waf --run "scratch/PA_957634_practicalAssignment --lossMod=$lossMod --stepWidth=$stepwidth --duration=$duration --packetsPerStep=$packetsPerStep --startPos=$startPos" > PA_957634_helper/PA_957634_log_${tnr}_$lossMod 2>&1
	bash PA_957634_raw2csv_$rawVersion.sh PA_957634_helper/PA_957634_log_${tnr}_$lossMod
	mv PA_957634_pa-0-0.pcap PA_957634_helper/PA_956634_${tnr}_${lossMod}_0-0.pcap
	mv PA_957634_pa-0-0.readable PA_957634_helper/PA_956634_${tnr}_${lossMod}_0-0.readable 2>/dev/null
	mv PA_957634_pa-1-0.pcap PA_957634_helper/PA_956634_${tnr}_${lossMod}_1-0.pcap
	mv PA_957634_pa-1-0.readable PA_957634_helper/PA_956634_${tnr}_${lossMod}_1-0.readable 2>/dev/null
}

rm -rf PA_957634_helper
rm -rf PA_957634_plots
mkdir PA_957634_helper
mkdir PA_957634_plots

echo "waf build"
./waf build

# 2.1
echo "2.1"
echo "  friis"
run 'f' '1' '351' '1' '0' '2_1' '2.1_3.1_4'
echo "  tworay"
run 't' '1' '351' '1' '0' '2_1' '2.1_3.1_4'
echo "  logdistance"
run 'l' '1' '351' '1' '0' '2_1' '2.1_3.1_4'

# 2.2
echo "2.2"
run 'j' '0' '3601' '1' '10' '2_2' '2.2'

# 3.1
echo "3.1"
run 'c' '1' '351' '1' '0' '3_1' '2.1_3.1_4'

#3.2
echo "3.2"
run 'c' '5' '7100' '100' '0' '3_2' '3.2'

# 4
echo "4"
run 's' '1' '351' '1' '0' '4' '2.1_3.1_4'

echo "plots"
gnuplot PA_957634_plot.plt
gnuplot PA_957634_plot22.plt
gnuplot PA_957634_plot32.plt

echo "plots to pdf via latex"
latexmk -pdf -interaction=nonstopmode PA_957634_plots_all.tex > PA_957634_plots_all_tex.log 2>&1
mv PA_957634_plots_all.pdf PA_957634_plots
mv PA_957634_plots_all.tex tmp
mv tmp PA_957634_plots_all.tex
