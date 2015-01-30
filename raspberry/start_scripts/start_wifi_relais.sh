#!/bin/bash
# use raspberry pi as infrastructure node for qaul.net

# uncomment the following line if you execute the script via /etc/rc.local
#sleep 60

SCRIPT_DIR="$(dirname "$(readlink -f ${BASH_SOURCE[0]})")"
echo $SCRIPT_DIR
cd $SCRIPT_DIR

# configure wifi
./wifi_qaul.sh wlan0 10.111.1.1
./wifi_qaul.sh wlan1 10.111.1.2

# start olsrd
./olsrd.sh wlan0 wlan1
