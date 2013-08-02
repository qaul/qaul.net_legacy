#!/bin/bash
# usage:
## ./wifi_ap_static.sh <INTERFACE> <IP>
## ./wifi_ap_static.sh ap0 10.104.10.30

# set interface ap0 for access point
# /etc/udev/rules.d/70-persistent-net.rules
# SUBSYSTEM=="net", ACTION=="add", DRIVERS=="?*", ATTR{address}=="00:13:33:b3:92:ec", ATTR{dev_id}=="0x0", ATTR{type}=="1", KERNEL=="wlan*", NAME="ap0"

# configure access point IP in 
# /etc/network/interfaces

# configure hostap
# ./hostapd.conf

# configure interface
sudo ifconfig $1 $2 netmask 255.0.0.0 broadcast 10.255.255.255 

# restart hostapd with multiple interfaces
sudo /etc/init.d/hostapd stop
sudo hostapd ./hostapd.conf &
sleep 5
