#!/bin/bash
## usage:
##   ./qaul.sh

cd /home/pi/bin

sleep 10

./qaul > /dev/null &
sleep 5

# download site to be registered as local host
wget http://127.0.0.1:8081/qaul.html
rm qaul.html
