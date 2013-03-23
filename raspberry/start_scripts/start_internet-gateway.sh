#!/bin/bash
# use raspberry pi as an Internet gateway and infrastructure node for qaul.net

# uncomment the following line if you execute the script via /etc/rc.local
#sleep 30

SCRIPT_DIR="$(dirname "$(readlink -f ${BASH_SOURCE[0]})")"
echo $SCRIPT_DIR
cd $SCRIPT_DIR

# configure wifi
./wifi_qaul.sh wlan0 10.111.1.1
./wifi_qaul.sh wlan1 10.111.1.2
#./wifi_dhcp.sh wlan2 wangba
# start olsrd
./olsrd.sh wlan0 wlan1
# start NAT for Internet sharing
./nat.sh eth0
