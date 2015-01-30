#!/bin/sh

## Configure wifi
## Enable (1) or disable (0) 
## <QAULWIFI> qaul.net IBSS mesh mode
## <LANWIFI> qaul.net_AP wifi access point
## <DISTANCE> set maximum antenna distance in meters.
##
## usage:
##     ./wifi_distance.sh <QAULWIFI> <LANWIFI> <DISTANCE>
##     ./wifi_distance.sh 1 0 600

# qaulwifi
if [ $1 == 1 ]; then
	uci set wireless.qaulwifi=wifi-iface
	uci set wireless.qaulwifi.device=radio0
	uci set wireless.qaulwifi.encryption=none
	uci set wireless.qaulwifi.network=qaulwifi
	uci set wireless.qaulwifi.ssid=qaul.net
	uci set wireless.qaulwifi.mode=adhoc
	uci set wireless.qaulwifi.bssid='02:11:87:88:D6:FF'
	uci set wireless.qaulwifi.disabled=0
else
	# uci set wireless.qaulwifi.disabled=1
	uci delete wireless.qaulwifi
fi

# lanwifi
if [ $2 == 1 ]; then
	uci set wireless.lanwifi=wifi-iface
	uci set wireless.lanwifi.device=radio0
	uci set wireless.lanwifi.encryption=none
	uci set wireless.lanwifi.network=lanwifi
	uci set wireless.lanwifi.ssid=qaul.net_AP
	uci set wireless.lanwifi.mode=ap
	uci set wireless.lanwifi.disabled=0
else
	#uci set wireless.lanwifi.disabled=1
	uci delete wireless.lanwifi
fi

# distance setup
if [ $3 > 0 ]; then
	uci set wireless.radio0.distance=$3
else
	uci set wireless.radio0.distance=300
fi

# activate configuration 
uci commit
/etc/init.d/network restart

exit 0
