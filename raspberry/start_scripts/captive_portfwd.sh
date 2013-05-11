#!/bin/bash
# start port forwarding for qaul.net's captive portal
# usage:
## ./captive_portfwd.sh 

SCRIPT_DIR="$(dirname "$(readlink -f ${BASH_SOURCE[0]})")"

# forward DHCP requests
sudo $SCRIPT_DIR/portfwd -c $SCRIPT_DIR/portfwd.conf 
