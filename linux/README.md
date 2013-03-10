Linux Instructions
==================

qaul.net has been tested on:
* Ubuntu 11.10: Oneiric Ocelot
* Ubuntu 12.04: Precise Pangolin

It should run on all recent Debian & Ubuntu distributions.


Installation and Compile Instructions
--------------------------------------

Compile olsrd

    cd olsrd_0.6.2
    make
    cp olsrd ../linux/qaul-build-desktop/
    cd ../

Compile olsrd_qaul plugin

    cd olsrd_0.6.2/lib/olsrd_qaul
    make
    cp olsrd_qaul.so.0.1 ../../../linux/qaul-build-desktop/
    # you need to install the shared library
    sudo make install
    cd ../../../

Compile pjsip library for VoIP

    cd pjproject-2.0.1
    ./configure
    make dep
    make

Compile static qaullib

    cd libqaul
    make OS=linux
    cd ../

qaul.net is implemented in C++ in QT. 

* To be able to develop qaul.net on Linux, install the latest QT Creator (QT's IDE).
* Open QT project and run the software.


