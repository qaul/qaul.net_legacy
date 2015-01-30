qaul.net Tinc VPN
=================

qaul.net runs a tinc VPN under hubch.qaul.net to tie networks together.
You may run your own tinc VPN on an Internet Server to join networks.


Configure Tinc on your OpenWRT Router
-------------------------------------

The basic setup is already prepared in the qaul.net openWRT image.

* tinc should be installed (only on devices with very little flash it is missing)
* The tinc folder structure is present at /etc/tinc/qaulvpn
* The tinc up and down scripts are configured in /etc/tinc/qaulvpn/tinc-up and 
  /etc/tinc-down
* A customizable configuration file is available at /etc/config/tinc
* The 'qaulvpn' network interface is already prepared in /etc/config/network and
  /etc/config/firewall


### Install tinc (if not already installed)

    opkg update
    opkg install tinc


### Create your public and private key

Login to your router and enter the following commands:

    # change to the tinc network location
    cd /etc/tinc/qaulvpn
    # create the keys
    # (answer all the questions with the enter key)
    tincd -n qaulvpn -K
    # copy the public key to the hosts folder
    # you must give it a unique name
    # only alphanumeric characters and under score are allowed
    cp rsa_key.pub hosts/YourTincDeviceName
    

### Configure and enable tinc

Edit the file /etc/config/tinc on your router and uncomment and edit 
the following lines:

    # set enabled to 1
    option enabled 1
    
    # Enter the same unique name as your public key has
    option Name YourTincDeviceName
    
    # Enter a line for every server tinc shall actively connect to.
    # You have to put the public key of the Tinc server in to the
    # /etc/tinc/qaulvpn/hosts directory. And the public key of this
    # router has to be in the tinc hosts directory of the Server you
    # want to connect to.
    list ConnectTo qaulHubCH


Tinc will now start automatically after a reboot.


Configure your Server on the Internet
-------------------------------------

* Install Tinc VPN
* Create your private key.
* Put all public keys in the folder /etc/tinc/qaulvpn/hosts
* Configure a tap interface at tinc startup and remove it at shut-down
* Install and run olsrd on 
* Configure your firewall to allow olsrd and qaul.net network traffic 
  on the qaulvpn interface.
