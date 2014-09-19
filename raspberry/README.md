Raspberry Pi Instructions
=========================

qaul.net runs as CLI client on Raspberry Pi.
It was tested on Raspberrys Debian Wheezy.


Installation and Compile Instructions
--------------------------------------

Compile olsrd

    # install required package
    sudo apt-get install bison flex
    cd olsrd_0.6.6.2
    
    # compile olsrd
    make
    # install olsrd
    sudo make install
    cd ../

Compile olsrd_qaul plugin

    cd olsrd_0.6.6.2/lib/olsrd_qaul
    make
    # install the shared library
    sudo make install
    cd ../../../

Compile dynamic gateway plugin

    cd olsrd_0.6.6.2/lib/dyn_gw
    make
    # install the shared library
    sudo make install
    cd ../../../

Compile qaul CLI

    cd cli
    make OS=raspberry VoIP=no
    cd ../


### Compile with VoIP

Compile olsrd, olsrd_qaul and the dynamic gateway plugin following 
the instructions above. Then follow this instructions here.


Compile pjsip library for VoIP

    # install required libraries
    sudo apt-get install build-essential automake autoconf libtool libasound2-dev libpulse-dev libssl-dev libsamplerate0-dev libcommoncpp2-dev libccrtp-dev libzrtpcpp-dev libdbus-1-dev libdbus-c++-dev libyaml-dev libpcre3-dev libgsm1-dev libspeex-dev libspeexdsp-dev libcelt-dev  libavformat-dev libavutil-dev libavcodec-dev libswscale-dev uuid-dev

    # compile pjsip
    cd pjproject-2.2.1
    ./configure

open the file pjlib/include/pj/config_site.h and enter

    #define PJ_IS_LITTLE_ENDIAN 1 
    #define PJ_IS_BIG_ENDIAN 0

compile pjsip

    make dep
    make
    cd ../

Compile qaul CLI

    cd cli
    make OS=raspberry
    cd ../


