Raspberry Pi Startup
====================

When building the qaul.net CLI binary all needed files should
have been copied to /home/pi/bin . All binaries and scripts will
be executed from this directory.


Change the default dynamic DHCP IP of your Raspberry to a static
IP. Edit therefore the file /etc/network/interfaces .

    #iface eth0 inet dhcp
    iface eth0 inet static
        address 10.11.12.13
        netmask 255.0.0.0
        gateway 10.255.255.255


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

