#!/bin/bash
# start port forwarding for qaul.net's captive portal
# usage:
## ./captiv_iptables.sh <INTERFACE> <IP>
## ./captiv_iptables.sh wlan0 10.111.1.10

SCRIPT_DIR="$(dirname "$(readlink -f ${BASH_SOURCE[0]})")"

# forward http
sudo iptables -t nat -I PREROUTING 1 -i $1 -p tcp -d $2 --dport 80 -j REDIRECT --to-port 8081 

# forward dns
sudo iptables -t nat -I PREROUTING 1 -i $1 -p udp -d $2 --dport 53 -j REDIRECT --to-port 8053 
