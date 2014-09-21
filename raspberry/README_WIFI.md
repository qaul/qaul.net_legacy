Raspberry Pi server via Wifi
============================

Copy all startup scripts to your preferred script location:

	mkdir -p /home/pi/bin
	cd raspberry
	cp start_scripts/* /home/pi/bin/
	cd ../


Edit one of the startup scripts according to your needs:

* start_wifi_internet-gateway.sh 
  Raspberry is an Internet gateway.
* start_wifi_qaul_internet-gateway.sh 
  Raspberry is a qaul user and an Internet gateway.
* start_wifi_qaul.sh 
  Raspberry is a qaul user.
* start_wifi_relais.sh 
  Raspberry is an olsr infrastructure relais node.

Edit olsrd.conf to load all the needed plugins.

Edit qaul.net data base /home/pi/qaul.net/cli/qaullib.db (set IP, set name)

Set auto start /etc/rc.local

    # open local.rc in editor
    sudo vi /etc/rc.local
    # insert the following line at the end of the file.
    # use the name and path of your specific start script.
    /home/pi/bin/start.sh


configure wifi interface
------------------------

If you need the wifi interfaces to be configured in a specific order, define interface 
names for specific MAC addresses in /etc/udev/rules.d/70-persistent-net.rules

    SUBSYSTEM=="net", ACTION=="add", DRIVERS=="?*", ATTR{address}=="00:13:33:b3:92:ec", ATTR{dev_id}=="0x0", ATTR{type}=="1", KERNEL=="wlan*", NAME="qaul0"

