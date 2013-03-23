#!/bin/bash
# usage:
## ./wifi_client.sh <INTERFACE> <NETWORKNAME>
## ./wifi_client.sh wlan0 wangba
sudo iwlist $1 scan | grep $2 
sudo iwconfig $1 essid $2
sudo dhclient $1
