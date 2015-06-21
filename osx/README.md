OSX Instructions
================

qaul.net works on:
* OSX 1.5 Leopard (needs special build)
* OSX 1.6 Snow Leopard
* OSX 1.7 Lion
* OSX 1.8 Mountain Lion
* OSX 1.9 Mavericks


Installation and Compile Instructions
--------------------------------------

Copy helper binaries, libraries & configuration files to needed locations

    # create location
    sudo mkdir /Library/qaul.net
    # copy olsrd, socat & configuration files
    cd osx
    sudo cp olsrd /Library/qaul.net/
    sudo cp olsrd_qaul.so.0.1 /Library/qaul.net/
    sudo cp olsrd_dyn_gw.so.0.5 /Library/qaul.net/
    sudo cp olsrd_osx.conf /Library/qaul.net/
    sudo cp olsrd_osx_gw.conf /Library/qaul.net/
    sudo cp socat /Library/qaul.net/
    cd ../


Compile qaulhelper
* Open the Xcode project qaulhelper.xcodeproj in Xcode to build the qaulhelper application.
* Copy the qaulhelper binary to /Library/qaul.net/
* Add the SUID flag to the qaulhelper binary

    # change group and owner to root
    sudo chown root /Library/qaul.net/qaulhelper
    sudo chgrp wheel /Library/qaul.net/qaulhelper
    # set SUID rights
    sudo chmod 6755 /Library/qaul.net/qaulhelper
    

Compile pjsip library for VoIP

    cd pjproject-2.3
    ./aconfigure --disable-ssl --disable-ffmpeg CFLAGS='-Wno-unused-label -m32' LDFLAGS='-m32'
    make dep
    make
    cd ../

Now you should be able to successfully compile libqaul   

    cd libqaul
    make OS=osx
    cd ../

Open the Xcode project "qaul_osx10.XY.xcodeproj" for your OS version in Xcode to build the qaul.net 
application.


### Optional

Compile olsrd

    cd olsrd-0.6.0
    make
    cp olsrd ../osx/
    cd ../

Compile olsrd_qaul plugin

    cd olsrd-0.6.0/lib/olsrd_qaul
    make
    cp olsrd_qaul.so.0.1 ../../../osx/
    # you need to install the shared library in /usr/lib/
    sudo make install
    cd ../../../
    

Installer
---------

To build the qaul.net package installer you need to download and install the free and
open source program "packages":
http://s.sudre.free.fr/Software/Packages/about.html

Put the applications (qaul.app, qaulhelper, olsrd, olsrd plugins, olsrd configuration 
files, socat) to their locations:

    /Applications
        /qaul.app
    /Library
        /qaul.net
            /olsrd
            /olsrd_dyn_gw.so.0.5
            /olsrd_osx.conf
            /olsrd_osx_gw.conf
            /olsrd_qaul.so.0.1
            /qaulhelper
            /socat

Open the project file osx/installer/qaul.net with "packages" and build the installer 
by selecting "Build > Build" from the menu.
