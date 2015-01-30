#!/bin/sh

## set ethernet to wan

# delete eth0 setup
uci delete network.qauleth
uci delete network.lan
uci delete network.wan

# set wan setup
uci set network.wan=interface
uci set network.wan.ifname=eth0
uci set network.wan.proto=dhcp

# activate configuration 
uci commit
echo "wan" > "/qaul/config/eth"
/etc/init.d/network restart
/etc/init.d/olsrd restart

exit 0
