#!/bin/sh

## configure 3G/4G mobile Internet
## 
## usage:
##     mobile_config.sh <apn> <pin> [<username> <password>]
##     mobile_config.sh surf.kabeld 1234

# set network connection
uci set network.wan3g=interface
uci set network.wan3g.proto=3g
uci set network.wan3g.service=umts
#uci set network.wan3g.maxwait=30
#uci set network.wan3g.defaultroute=0
# check correct path ( /dev/ttypACM0 | /dev/ttyUSB0 )
uci set network.wan3g.device=/dev/ttyUSB0
# user input
uci set network.wan3g.apn=$1
uci set network.wan3g.pincode=$2
uci set network.wan3g.username=$3
uci set network.wan3g.password=$4

uci commit

echo "1" > "/qaul/config/mobile"

exit 0
