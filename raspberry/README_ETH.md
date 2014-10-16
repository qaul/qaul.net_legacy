Raspberry Pi Server via Ethernet Network Interface
==================================================

When building the qaul.net CLI binary all needed files should
have been copied to /home/pi/bin . All binaries and scripts will
be executed from this directory.


Configure qaul
--------------

Start qaul binary via CLI

    cd /home/pi/bin
    ./qaul


Open the qaul user interface in your browser and configure 
language, user name and file sharing options.
http://IpOfYourRaspberryPi:8081/qaul.html

Stop/kill the running qaul binary now.

Open the newly created sqlite db file /home/pi/bin/qaullib.db and 
set the correct IP address.

    sqlite3 /home/pi/bin/qaullib.db
    # show currently configured IP
    SELECT value FROM 'config' WHERE key = "ip";
    # delete old IP
    DELETE FROM 'config' WHERE key = "ip";
    # set your IP
    INSERT INTO 'config' ('key','type','value') VALUES ("ip",1,"10.11.12.13");
    .exit


Configure Ethernet Network Interface
------------------------------------

Change the default dynamic DHCP IP of your Raspberry to a static
IP. Edit therefore the file /etc/network/interfaces and configure
according to this example:

    #iface eth0 inet dhcp
    iface eth0 inet static
        address 10.11.12.13
        netmask 255.0.0.0


Configure Startup Scripts
-------------------------

Edit one of the startup scripts according to your needs. Change
the example IP to your IP:

* start_eth_qaul.sh 
  Raspberry Pi is a qaul.net server user.
  

Set auto start /etc/rc.local

    # open local.rc in editor
    sudo vi /etc/rc.local
    # insert the following line at the end of the file.
    # use the name and path of your specific start script.
    /home/pi/bin/start_eth_qaul.sh


Reboot your raspberry and it should serve qaul.net over the Ethernet
network interface.
