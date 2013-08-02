#!/bin/bash
# usage:
## wifi_qaul.sh <INTERFACE> <IP>
## wifi_qaul.sh wlan0 10.111.111.1
sudo iw dev $1 set type ibss
sudo ip link set $1 up
sleep 2
sudo iwconfig $1 essid 'qaul.net'
sudo ifconfig $1 $2 netmask 255.0.0.0 broadcast 10.255.255.255

sleep 2
