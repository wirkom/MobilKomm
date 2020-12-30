set terminal png
set output "plots/plot2.2.png"
set title "Task 2.2 Small Scale Fading"
set xlabel "time (s)"
set xrange [-10:3810]
set ylabel "RSS"
set term png size 2000, 1500
set grid x y
set style boxplot outliers pointtype 7
set style data boxplot

set datafile missing "-nan"
plot "helper/log_2_2_j.csv" index 0 title "JakesPropagationLossModel" with linespoints ps 1, "helper/log_2_2_j.csv" using (3700):1:(50.0) title "JakesPropagationLossModel in a Boxplot"
