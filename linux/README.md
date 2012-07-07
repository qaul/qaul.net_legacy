Linux Instructions
==================

qaul.net was tested on:
* Ubuntu 11.10: Oneiric Ocelot

It should work on all recent Debian & Ubuntu distributions.


Installation and Compile Instructions
--------------------------------------

qaul.net is implemented in C++ in QT. 

* To be able to develop qaul.net on Linux, install the latest QT Creator (QT's IDE).
* Open QT project and run the software.


### Optional:

Compile olsrd

    cd olsrd_0.6.2
    make

Compile olsrd_qaul plugin

    cd olsrd_0.6.2/lib/olsrd_qaul
    make

Compile static qaullib

    cd libqaul
    make linux
