#!/bin/bash
# restart dhcp server
# usage:
## ./captive_dhcp.sh

SCRIPT_DIR="$(dirname "$(readlink -f ${BASH_SOURCE[0]})")"

# for the captive portal, configure the following dhcp server files:
# ( udhcpd allows only one interface )
# /etc/udhcpd.conf
# enable udhcp server in 
# /etc/default/udhcpd
#
# restart dhcp server
sudo /etc/init.d/udhcpd restart
