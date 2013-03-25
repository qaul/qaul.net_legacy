Raspberry Pi Startup
====================

Copy all startup scripts to your preferred script location:

	mkdir /home/pi/bin
	cp * /home/pi/bin/


Edit one of the startup scripts according to your needs:

* start_internet-gateway.sh 
  Raspberry is an Internet gateway.

Edit olsrd.conf to load all the needed plugins.

Edit qaul.net data base /home/pi/qaul.net/cli/qaullib.db (set IP, set name)

Set auto start /etc/rc.local

    # open local.rc in editor
    sudo vi /etc/rc.local
    # insert the following line at the end of the file.
    # use the name and path of your specific start script.
    /home/pi/bin/start.sh