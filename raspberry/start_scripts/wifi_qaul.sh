#!/bin/bash
# usage:
## wifi_qaul.sh <INTERFACE> <IP>
## wifi_qaul.sh wlan0 10.111.111.1
sudo ip link set $1 down
sudo iwconfig $1 mode ad-hoc
sudo iwconfig $1 channel 11
sudo iwconfig $1 essid 'qaul.net'
#sudo iwconfig $1 ap 02:11:87:88:D6:FF
sudo ip link set $1 up
sudo ip addr add $2/24 dev $1
