#!/bin/bash
# stop qaul.net

sudo killall qaul
sudo killall olsrd

# stop port forwarding
#sudo killall portfwd
sudo iptables -t nat -D PREROUTING 1 
