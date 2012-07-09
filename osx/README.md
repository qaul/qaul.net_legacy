OSX Instructions
================

qaul.net works on:
* OSX 1.5 Leopard
* OSX 1.6 Snow Leopard
* OSX 1.7 Lion


Installation and Compile Instructions
--------------------------------------

Compile static qaullib

    # uncomment in libqaul/Makefile the line '#CFLAGS=-arch i386' to 'CFLAGS=-arch i386'
    cd libqaul
    make osx
    cd ../

Open the OSX project in Xcode to run qaul.net


optional:

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
