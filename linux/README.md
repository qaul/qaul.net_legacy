Linux Instructions
==================

qaul.net was tested on:
* Ubuntu 11.10: Oneiric Ocelot

It should work on all recent Debian & Ubuntu distributions.


Installation and Compile Instructions
--------------------------------------

Compile olsrd

    cd olsrd_0.6.2
    make
    cd ../

Compile olsrd_qaul plugin

    cd olsrd_0.6.2/lib/olsrd_qaul
    make
    cd ../../../

Compile static qaullib

    cd libqaul
    make linux
    cd ../

Compile pjp library for VoIP

    cd pjproject-2.0.1
    ./configure
    make dep
    make

qaul.net is implemented in C++ in QT. 

* To be able to develop qaul.net on Linux, install the latest QT Creator (QT's IDE).
* Open QT project and run the software.


