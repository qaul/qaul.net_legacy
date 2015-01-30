#!/bin/sh

## switch internet sharing off

# configure firewall
uci delete firewall.qaul_wan

# configure olsrd
uci set olsrd.dyn_gw.ignore=1
uci commit

echo "0" > "/qaul/config/internet"

# restart services
/etc/init.d/olsrd restart
/etc/init.d/firewall restart

exit 0
