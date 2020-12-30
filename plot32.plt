set terminal png
set output "plots/plot3.2.png"
set title "3.2 Packet loss"
set xlabel "distance"
#set xrange [0:71.5]
#set x2range [0:71.5]
set xtics nomirror rotate 90
set ylabel "packet loss (in %)"
set y2label "RSS"
set y2tics
set ytics nomirror
set grid x y2
set style boxplot outliers pointtype 7
set style data boxplot
set term png size 2000, 1500

#set key outside center below
set datafile missing "-nan"
plot "helper/log_3_2_c.csv" using (0):1:(0.5):2 axes x1y2 title "RSS", "helper/log_3_2_packetloss.csv" index 0 title "packetloss" with linespoints ps 1 axes x1y1
