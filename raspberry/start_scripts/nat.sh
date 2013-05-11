#!/bin/bash
# usage:
# list all interfaces that should share Internet
## ./nat.sh <INTERFACE> [<INTERFACE2> [<INTERFACE3> ... ]]
## ./nat.sh eth0 wlan0
for i in $*
  do sudo iptables --table nat --append POSTROUTING --out-interface $i -j MASQUERADE
done
sudo iptables -t nat -L
