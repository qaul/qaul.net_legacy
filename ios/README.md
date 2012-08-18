iOS Instructions
================

qaul.net tested on:
* iOS 5.1.1 iphone 4


Installation and Compile Instructions
--------------------------------------

Compile pjsip

    cd pjproject-2.0.1
    ./configure-iphone
    
open the file pjlib/include/pj/config_site.h and enter

    #define PJ_CONFIG_IPHONE 1
    #include <pj/config_site_sample.h>

Now build pjsip
    
    make dep
    make
    cd ../


Compile qaullib: 
Open the qaullib Xcode project ios/qaullib.xcodeproj and build the static library.

Compile iOS: 
Open the iOS project in Xcode to run qaul.net


optional:

Compile olsrd
Open the ios Makefile olsrd-0.6.0/make/Makefile.ios and configure SDK_VERSION

    cd olsrd-0.6.0
    make OS=ios
    cp olsrd ../ios/
    cd ../

Compile olsrd_qaul plugin

    cd olsrd-0.6.0/lib/olsrd_qaul
    make OS=ios
    cp olsrd_qaul.so.0.1 ../../../ios/
    cd ../../../
