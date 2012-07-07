OSX Instructions
================

qaul.net works on:
* OSX 1.5 Leopard
* OSX 1.6 Snow Leopard
* OSX 1.7 Lion


Installation and Compile Instructions
--------------------------------------

Compile olsrd

    cd olsrd-0.6.0
    make

Compile olsrd_qaul plugin

    cd olsrd-0.6.0/lib/olsrd_qaul
    make

Compile static qaullib
* uncomment in libqaul/Makefile the line '#CFLAGS=-arch i386' to 'CFLAGS=-arch i386' 

    cd libqaul
    make osx

Open the OSX project in Xcode

    
