#!/bin/sh

## set ethernet to wan

# configure firewall
uci set firewall.lan_wan=forwarding
uci set firewall.lan_wan.src=lan
uci set firewall.lan_wan.dest=wan

uci set firewall.lan_qaul=forwarding
uci set firewall.lan_qaul.src=lan
uci set firewall.lan_qaul.dest=qaul

# delete eth0 setup
uci delete network.wan
uci delete network.qauleth
uci delete network.lan

# set lan setup
uci set network.lan=interface
uci set network.lan.ifname=eth0
uci set network.lan.proto=static
uci set network.lan.ipaddr=192.168.171.1
uci set network.lan.netmask=255.255.255.0
uci set network.lan.broadcast=192.168.171.255

# activate configuration 
uci commit
echo "lan" > "/qaul/config/eth"
/etc/init.d/network restart
/etc/init.d/olsrd restart

exit 0
