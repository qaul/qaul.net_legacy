#!/bin/bash
## use raspberry pi as a qaul.net server over Ethernet 
## network interface.
## 
## Configure the Ethernet network interface in 
## /etc/network/interfaces
## 
## replace the default DHCP configuration
## 
##     iface eth0 inet dhcp
## 
## with a static IP configuration
## 
##     iface eth0 inet static
##         address 10.11.12.13
##         netmask 255.0.0.0
##         gateway 10.255.255.255
## 

# uncomment the following line if you execute the script via /etc/rc.local
sleep 60

SCRIPT_DIR="$(dirname "$(readlink -f ${BASH_SOURCE[0]})")"
echo $SCRIPT_DIR
cd $SCRIPT_DIR

# start olsrd
sudo olsrd -i eth0 -f $SCRIPT_DIR/olsrd_qaul.conf -d 0

# start qaul
sudo -u pi ./qaul.sh

# start captive portal
./captive_iptables.sh eth0 10.11.12.13
#./captive_portfwd.sh 
#./captive_dhcp.sh 
