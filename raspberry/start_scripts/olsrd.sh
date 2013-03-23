#!/bin/bash
# usage:
## ./olsrd.sh <INTERFACE> [<INTERFACE2> [<INTERFACE3> ... ]]
## ./olsrd.sh wlan1 wlan2 wlan3 wlan4

SCRIPT_DIR="$(dirname "$(readlink -f ${BASH_SOURCE[0]})")"

sudo olsrd -f $SCRIPT_DIR/olsrd.conf -i $* -d 0
