#!/bin/bash
# usage:
## ./wifi_ap.sh <OLSRD_INTERFACES>

# set interface ap0 for access point
# /etc/udev/rules.d/70-persistent-net.rules
# SUBSYSTEM=="net", ACTION=="add", DRIVERS=="?*", ATTR{address}=="00:13:33:b3:92:ec", ATTR{dev_id}=="0x0", ATTR{type}=="1", KERNEL=="wlan*", NAME="ap0"

# configure access point IP in 
# /etc/network/interfaces

# configure udhcp server in
# /etc/udhcpd.conf
# enable udhcp server in 
# /etc/default/udhcpd

# configure interface
sudo ifconfig ap0 192.168.111.1 netmask 255.255.255.0 broadcast 192.168.111.255 

# restart dhcp server
sudo /etc/init.d/udhcpd restart

# start nat
./nat.sh $*
