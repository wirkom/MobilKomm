#!/bin/sh
tcpdump -nn -tt -r pa-0-0.pcap > pa-0-0.readable 2>\dev\null
cat pa-0-0.readable | sed 's/\.[0-9]* IP.*//' > helper/pa-0-0.short

tcpdump -nn -tt -r pa-1-0.pcap > pa-1-0.readable 2>\dev\null
cat pa-1-0.readable | sed 's/\.[0-9]* IP.*//' > helper/pa-1-0.short

rm -rf tmpHelper
touch tmpHelper
for i in $(seq 0 70); do echo $i >> tmpHelper; done

diff helper/pa-0-0.short helper/pa-1-0.short | grep "^<" | sed 's/< //' | awk -F" "  '{$1-=1;print}' | sed 's/[0-9][0-9]$//' >> tmpHelper
sort -g tmpHelper | uniq -c | sed 's/^\s*\([0-9]*\) \([0-9]*\)/\1 \2/' | awk -F" "  '{$1-=1;$2=($2+1)*5;print}' | sed 's/ /,/' > helper/log_3_2_packetloss.csv

rm tmpHelper

sh raw2csv_2.1_3.1_4.sh helper/log_3_2_c
cat helper/log_3_2_c.csv | sed 's/,/ /' > helper/log_3_2_c.csv_tmp
mv helper/log_3_2_c.csv_tmp helper/log_3_2_c.csv

