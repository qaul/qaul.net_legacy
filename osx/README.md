OSX Instructions
================

qaul.net works on:
* OSX 1.5 Leopard
* OSX 1.6 Snow Leopard
* OSX 1.7 Lion


Installation and Compile Instructions
--------------------------------------

Compile pjp library for VoIP

    cd pjproject-2.0.1
    ./aconfigure --disable-ssl CFLAGS='-Wno-unused-label -m32' LDFLAGS='-m32'
    # FIXME: decide on which is working and best
    # or do this (from https://github.com/eofster/Telephone)
    #CFLAGS="-O2 -arch i386 -arch x86_64" ./configure --disable-ssl
    make dep
    make
    cd ../

FIXME: is that correct?
You may need to install libssl and libcrypto (from mac ports /opt/loca/lib/libssl.a )


Compile static qaullib

    # uncomment in libqaul/Makefile the line '#OSX_CFLAGS=-arch i386' to 'OSX_CFLAGS=-arch i386'
    cd libqaul
    make OS=osx
    cd ../

Open the OSX project in Xcode to run qaul.net


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
    cd ../../../
