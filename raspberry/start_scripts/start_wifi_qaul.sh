#!/bin/bash
# use raspberry pi as an Internet gateway and run qaul.net

# uncomment the following line if you execute the script via /etc/rc.local
#sleep 60

SCRIPT_DIR="$(dirname "$(readlink -f ${BASH_SOURCE[0]})")"
echo $SCRIPT_DIR
cd $SCRIPT_DIR

# configure wifi
./wifi_qaul.sh wlan0 10.111.1.10
./wifi_qaul.sh wlan1 10.111.1.11

# start olsrd
./olsrd.sh wlan0 wlan1

# start qaul
sudo -u pi ./qaul.sh

# start captive portal
./captive_iptables.sh wlan0 10.111.1.10
./captive_iptables.sh wlan1 10.111.1.11
#./captive_portfwd.sh 
./captive_dhcp.sh 
