#!/bin/bash
# restart dhcp server
# usage:
## ./captve_dhcp.sh

SCRIPT_DIR="$(dirname "$(readlink -f ${BASH_SOURCE[0]})")"

# for the captive portal, configure the following dhcp server files:
# /etc/default/isc-dhcp-server
# /etc/dhcp/dhcpd.conf
#
# restart dhcp server
sudo /etc/init.d/isc-dhcp-server restart
