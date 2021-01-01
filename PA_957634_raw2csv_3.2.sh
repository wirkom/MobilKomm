#!/bin/sh
tcpdump -nn -tt -r PA_957634_pa-0-0.pcap > PA_957634_pa-0-0.readable 2>\dev\null
cat PA_957634_pa-0-0.readable | sed 's/\.[0-9]* IP.*//' > PA_957634_helper/PA_957634_pa-0-0.short

tcpdump -nn -tt -r PA_957634_pa-1-0.pcap > PA_957634_pa-1-0.readable 2>\dev\null
cat PA_957634_pa-1-0.readable | sed 's/\.[0-9]* IP.*//' > PA_957634_helper/PA_957634_pa-1-0.short

rm -rf tmpHelper
touch tmpHelper
for i in $(seq 0 70); do echo $i >> tmpHelper; done

diff PA_957634_helper/PA_957634_pa-0-0.short PA_957634_helper/PA_957634_pa-1-0.short | grep "^<" | sed 's/< //' | awk -F" "  '{$1-=1;print}' | sed 's/[0-9][0-9]$//' >> tmpHelper
sort -g tmpHelper | uniq -c | sed 's/^\s*\([0-9]*\) \([0-9]*\)/\1 \2/' | awk -F" "  '{$1-=1;$2=($2+1)*5;print}' | sed 's/ /,/' > PA_957634_helper/PA_957634_log_3_2_packetloss.csv

rm tmpHelper

sh PA_957634_raw2csv_2.1_3.1_4.sh PA_957634_helper/PA_957634_log_3_2_c
cat PA_957634_helper/PA_957634_log_3_2_c.csv | sed 's/,/ /' > PA_957634_helper/PA_957634_log_3_2_c.csv_tmp
mv PA_957634_helper/PA_957634_log_3_2_c.csv_tmp PA_957634_helper/PA_957634_log_3_2_c.csv

