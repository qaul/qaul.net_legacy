#!/bin/sh

## switch internet sharing on

# configure firewall
uci set firewall.qaul_wan=forwarding
uci set firewall.qaul_wan.src=qaul
uci set firewall.qaul_wan.dest=wan

# configure olsrd
uci set olsrd.dyn_gw.ignore=0
uci commit

echo "1" > "/qaul/config/internet"

# restart olsrd
/etc/init.d/olsrd restart
/etc/init.d/firewall restart

exit 0
