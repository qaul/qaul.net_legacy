#!/bin/bash
# start port forwarding for qaul.net's captive portal
# usage:
## ./portfwd.sh <IP> [<IP> ... [<IP>]]
## ./portfwd.sh 10.111.1.10 10.111.1.10

SCRIPT_DIR="$(dirname "$(readlink -f ${BASH_SOURCE[0]})")"

# forward http
for i in $*
  do sudo iptables -t nat -I PREROUTING 1 -p tcp -d $2 --dport 80 -j REDIRECT --to-port 8081 
done

# forward dns
for i in $*
  do sudo iptables -t nat -I PREROUTING 1 -p udp -d $2 --dport 53 -j REDIRECT --to-port 8053 
done

sudo iptables -t nat -L

# forward DHCP requests
$SCRIPT_DIR/portfwd -c $SCRIPT_DIR/portfwd.conf 
