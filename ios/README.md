iOS Instructions
================

qaul.net tested on:
* iOS 5.1.1 iphone 4


Prerequisites
-------------

The following setup was used to

* Xcode 4.5
* iOSOpenDev http://iosopendev.com/download/
* The iPhone needs to be jailbreaked


Installation and Compile Instructions
--------------------------------------

Compile pjsip

    cd pjproject-2.0.1
    ./configure-iphone
    
open the file pjlib/include/pj/config_site.h and enter

    #define PJ_CONFIG_IPHONE 1
    #define PJ_IS_LITTLE_ENDIAN 1 
    #define PJ_IS_BIG_ENDIAN 0
    #include <pj/config_site_sample.h>

Now build pjsip
    
    make dep
    make
    cd ../


Compile qaullib: 
* Open the qaullib Xcode project ios/qaullib.xcodeproj and build the static library.
* Copy the newly built static libqaullib.a library to the ios directory

    cd ios
    cp ~/Library/Developer/Xcode/DerivedData/qaullib-{RANDOM}/Build/Intermediates/qaullib.build/Release-iphoneos/qaullib.build/Objects-normal/armv7/libqaullib.a ./
    cd ../


Compile iOS
* Open the iOS project in Xcode
* Make sure you can SSH to your iPhone from your developer machine
* Change target > Build Settings > User-Defined > iOSOpenDevDevice from "iqaul.local" to 
  your device name or device IP.
* Perform a “Build For Profiling” ( cmd + shift + i ) to build and install qaul.net via
  iOSOpenDev
* Start qaul.net app manually on your iPhone

Watch the log output on your iPhone
* Install syslogd from cydia store on your iPhone (restart the device after installation)
* Create a SSH over USB connection to your iPhone (e.g. via iTools 
  http://itools.hk/cms/webpage32.htm )
* Display the logs:

    tail -f /var/log/syslog


Optional: compile olsrd and olsrd_qaul plugin
---------------------------------------------

Requirements
* install iOSOpenDev http://iosopendev.com/download/

Compile olsrd
Open the ios Makefile olsrd-0.6.0/make/Makefile.ios and configure SDK_VERSION

    cd olsrd-0.6.0
    make OS=ios
    # sign the executable
    /opt/iOSOpenDev/bin/iosod sign ./olsrd
    cp olsrd ../ios/Package/usr/bin/
    cd ../

Compile olsrd_qaul plugin

    cd olsrd-0.6.0/lib/olsrd_qaul
    make OS=ios
    cp olsrd_qaul.0.1.dylib ../../../ios/qaul.net/Package/usr/lib/
    cd ../../../


Compile qaulhelper
* Open the qaulhelper project in Xcode
* Build the project with iOSOpenDev (CMD - SHIFT - i)
* Copy the executable into the qaul.net project and set suid bit

    cd ios
    cp qaulhelper/qaulhelper/Package/usr/bin/qaulhelper qaul.net/Package/usr/bin/
    chmod 6755 qaul.net/Package/usr/bin/qaulhelper
    cd ../
