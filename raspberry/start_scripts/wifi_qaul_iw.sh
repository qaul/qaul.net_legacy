#!/bin/bash
# usage:
## wifi_qaul_iw.sh <INTERFACE> <IP>
## wifi_qaul_iw.sh wlan0 10.111.111.1

# put card into ibss mode
sudo iw dev $1 set type ibss

sudo ip link set $1 up
sleep 2

# create or join network
# channel 1 : 2412
# channel 11: 2462
sudo iw dev $1  ibss join qaul.net 2462

sudo ifconfig $1 $2 netmask 255.0.0.0 broadcast 255.255.255.255

sleep 2

