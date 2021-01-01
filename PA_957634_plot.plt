set terminal png
set output "PA_957634_plots/PA_957634_plot2.1_3.1_4.png"
set title "Task 2.1, 3.1 and 4"
set xlabel "Distance (m)"
set xrange [-1:351]
set ylabel "RSS (dbm)"
set term png size 2000, 1500
set grid x y

set datafile missing "-nan"
plot "PA_957634_helper/PA_957634_log_2_1_f.csv" index 0 title "FriisPropagationLossModel" with linespoints ps 1, "PA_957634_helper/PA_957634_log_4_s.csv" index 0 title "NewTwoRayGroundPropagationLossModel" with linespoints ps 1, "PA_957634_helper/PA_957634_log_2_1_t.csv" index 0 title "TwoRayGroundPropagationLossModel" with linespoints ps 1, "PA_957634_helper/PA_957634_log_2_1_l.csv" index 0 title "LogDistancePropagationLossModel" with linespoints ps 1, "PA_957634_helper/PA_957634_log_3_1_c.csv" index 0 title "combined LogDistancePropagationLossModel and JakesPropagationLossModel" with linespoints ps 1
