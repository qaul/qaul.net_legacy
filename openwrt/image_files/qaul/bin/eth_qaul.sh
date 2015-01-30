#!/bin/sh

## set ethernet to qaul.net olsr

# delete eth0 setup
uci delete network.wan
uci delete network.lan
uci delete network.qauleth

# set qaul setup
uci set network.qauleth=interface
uci set network.qauleth.ifname=eth0
uci set network.qauleth.proto=static
uci set network.qauleth.ipaddr=`cat /qaul/config/qauleth`
uci set network.qauleth.netmask=255.0.0.0
uci set network.qauleth.broadcast=10.255.255.255

# activate configuration 
uci commit
echo "qauleth" > "/qaul/config/eth"
/etc/init.d/network restart
/etc/init.d/olsrd restart

exit 0
