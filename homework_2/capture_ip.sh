#! /usr/bin/env bash
tcpdump -c 10 dst host 192.168 | awk '{print $3}' | sort | uniq > ip_captured &
curl google.com > /dev/null &
curl whu.edu.cn > /dev/null &
