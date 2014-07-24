Build Custom OpenWRT Image
==========================

To follow these instructions and build the custom OpenWRT qaul.net image
you need a 64 bit Linux computer. I executed it on a 64 bit Ubuntu 14.04.


Preparations
------------

### Download ImageBuilder

* Clone or download the qaul.net source code git repository 
  https://github.com/WachterJud/qaul.net
* Download the OpenWRT source code according to your routers CPU architecture.
  e.g. TL-MR3020, TL-WR841ND, TL-WR842ND, etc. have the architecture ar71xx
  download therefore 
  http://downloads.openwrt.org/barrier_breaker/14.07-rc1/ar71xx/generic/OpenWrt-ImageBuilder-ar71xx_generic-for-linux-x86_64.tar.bz2
* Extract the downloaded archive in the openwrt directory.
* To add locally build packages edit the file repositories.conf and add
  the path to your local repository:

    # e.g.
    src custom file:///PathToRepository/bin/ar71xx/packages


### Build From Scratch

    # install needed packages
    apt-get install libncurses5-dev libncurses5 zlib1g-dev subversion-tools \
    protobuf-compiler
    
    # clone git repository
    git clone git://nbd.name/openwrt.git
    cd openwrt
    
    # get latest feeds
    ./scripts/feeds update -a
    # install symlinks
    ./scripts/feeds install -a
    
    # configure the making (everything was fine on default)
    make menuconfig
    
    # build firmware 
    # (the -q option defines the number of threads used)
    make -q 4 V=s

* The builded images, the ImageBuilder and the SDK are located in: 
  bin/ar71xx
* The packages are located in:
  bin/ar71xx/packages


Build
-----

### Via Build script

Open a Terminal and execute the build script according to your router
model.

    # navigate to this folder
    cd openwrt
    
    # execute the build script according to your router model
    # TP-Link TL-WR842ND & TP-Link TL-WR842N
    ./WR842_build.sh


### Manually

Open a Terminal and follow the instructions

    # move into the extracted OpneWrt ImageBuilder folder.
    # e.g.
    cd openwrt/
    # list all available build profiles and search for the profile for
    # your router
    make info
    # e.g.
    # TLWR842:
    #       TP-LINK TL-WR842N/ND
    # 
    # use therefore TLWR842 as build profile in the next build step.


Build the image for your router. This is router specific. You need to 
fill in the profile for your router. There are also different package 
lists, as not all routers have the same amount of storage memory and 
can therefore store an image with all convenient packages.

    # Build the image for your device
    # 
    # for TL-WR842ND
    make image PROFILE=TLWR842 \
    FILES=../WR84X_qaul.net \
    PACKAGES="olsrd kmod-ipip ip \
    uhttpd \
    tinc \
    kmod-usb-storage block-mount kmod-fs-ext4 \
    openssh-sftp-server horst" 


Flash Router
------------

* Connect to your router and login in to the administration web interface.
* Navigate to 'System Tools' > 'Firmware upgrade'
* Upload the created binary that ends on 'squashfs-factory.bin'.


Configure Router
----------------

* At the first start up the IP address the qaul.net wifi IP address is 
  set randomly. 
* The router can be used right away. (But you should change the password!)
* To connect to the router: Start qaul.net on your computer. Select the 
  'gears tab' of the user interface and click on 'Show Network' to display
  the qaul.net network. Spot the IP address in the network display. Use
  this IP address to log in to your router and configure it.


### Via Shell

    # connect to your router via SSH as root user
    # you need to write your IP address instead of IPADDRESS
    ssh -l root IPADDRESS
    
    # change the password!
    passwd


### Via Web Interface

* Open the IP address of your router in a web bowser.
* Click on login. 
  Username: root 
  Password: qaul.net
* Change the default password first.

