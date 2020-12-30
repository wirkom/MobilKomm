set terminal png
set output "plots/plot2.1_3.1_4.png"
set title "Task 2.1, 3.1 and 4"
set xlabel "Distance (m)"
set xrange [-1:351]
set ylabel "RSS"
set term png size 2000, 1500
set grid x y

set datafile missing "-nan"
plot "helper/log_2_1_f.csv" index 0 title "FriisPropagationLossModel" with linespoints ps 1, "helper/log_4_s.csv" index 0 title "NewTwoRayGroundPropagationLossModel" with linespoints ps 1, "helper/log_2_1_t.csv" index 0 title "TwoRayGroundPropagationLossModel" with linespoints ps 1, "helper/log_2_1_l.csv" index 0 title "LogDistancePropagationLossModel" with linespoints ps 1, "helper/log_3_1_c.csv" index 0 title "combined LogDistancePropagationLossModel and JakesPropagationLossModel" with linespoints ps 1
