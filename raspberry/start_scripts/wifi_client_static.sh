#!/bin/bash
# usage:
## ./wifi_client.sh <INTERFACE> <IP> <NETWORKNAME>
## ./wifi_client.sh wlan0 10.104.10.30 wangba
sudo iwlist $1 scan | grep $3 
sleep 2
sudo iwconfig $1 essid $3
sudo ifconfig $1 $2 netmask 255.0.0.0 broadcast 10.255.255.255
