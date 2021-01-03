set terminal png
set output "PA_957634_plots/PA_957634_plot4.png"
set title "Task 4"
set xlabel "Distance (m)"
set xrange [-1:351]
set ylabel "RSS (dbm)"
set term png size 2000, 1500
set grid x y

set datafile missing "-nan"
plot "PA_957634_helper/PA_957634_log_4_s.csv" index 0 title "NewTwoRayGroundPropagationLossModel" with linespoints ps 1 linetype 2
