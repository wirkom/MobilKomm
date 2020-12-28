set terminal png
set output "plot.png"
set title "asdf"
set xlabel "Distance (m)"
set xrange [-1:350]
set ylabel "RSS"
set term png size 2000, 1500

set key outside center below
set datafile missing "-nan"
plot "log_2_1_f.csv" index 0 title "friis" with linespoints ps 1, "log_4.csv" index 0 title "selfmade" with linespoints ps 1, "log_2_1_t.csv" index 0 title "two" with linespoints ps 1, "log_2_1_l.csv" index 0 title "log" with linespoints ps 1, "log_2_1_c.csv" index 0 title "combined" with linespoints ps 1
