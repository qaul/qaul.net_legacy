#!/bin/bash
# usage:
## wifi_qaul.sh <INTERFACE> <IP>
## wifi_qaul.sh wlan0 10.111.111.1
sudo iwconfig $1 essid 'qaul.net'
sudo ifconfig $1 $2 netmask 255.0.0.0 broadcast 255.255.255.255

sleep 2
