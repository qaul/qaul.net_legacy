#!/bin/bash
# usage:
## ./wifi_ap.sh <OLSRD_INTERFACES>

# set interface ap0 for access point
# /etc/udev/rules.d/70-persistent-net.rules
# SUBSYSTEM=="net", ACTION=="add", DRIVERS=="?*", ATTR{address}=="00:13:33:b3:92:ec", ATTR{dev_id}=="0x0", ATTR{type}=="1", KERNEL=="wlan*", NAME="ap0"

# configure access point IP in 
# /etc/network/interfaces

# configure isc-dhcp-server server in
# /etc/default/isc-dhcp-server
# /etc/dhcp/dhcpd.conf

# configure interface
sudo ifconfig ap0 192.168.111.1 netmask 255.255.255.0 broadcast 192.168.111.255 

# restart dhcp server
sudo /etc/init.d/isc-dhcp-server restart

# start nat
./nat.sh $*
