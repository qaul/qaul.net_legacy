Raspberry Pi Instructions
=========================

qaul.net runs as CLI client on Raspberry Pi.
It was tested on Raspbian (Raspberry's Debian Wheezy).


Scenarios
---------

### qaul.net via Ethernet

Connect the Raspberry Pi via Ethernet with other clients, 
e.g. some wifi routers running OpenWRT.

Compile the binaries according to the instructions below.
Then configure the Raspberry Pi as described in [README_ETH.md](https://github.com/WachterJud/qaul.net/blob/master/raspberry/README_ETH.md)


### qaul.net via USB wifi (not recommended)

**Warning:** The USB wifi drivers for Ad-hoc (IBSS) wifi are all
very bad up to unusable. Atheros USB wifi drivers can only
handle up to 7 participants in the IBSS network, afterwards
the drivers will crash, leading to a crash of the Raspberry Pi. 
Realtek and Ralink spontanously change IBSS cell ID leading
to multiple non-merging IBSS networks, that are not able to 
communicate with each other anymore.

Connect the Raspberry Pi via USB wifi with other qaul.net clients.

Compile the binaries according to the instructions below.
Then configure the Raspberry Pi as described in [README_WIFI.md](https://github.com/WachterJud/qaul.net/blob/master/raspberry/README_WIFI.md)


Installation and Compile Instructions
--------------------------------------

Compile olsrd

    # install required package
    ````
    sudo apt-get install bison flex`
    cd olsrd-0.6.6.2
    ````
    
    # compile olsrd
    `make`
    # install olsrd
    ````
    sudo make install
    cd ../
    ````

Compile olsrd_qaul plugin

    ````
    cd olsrd-0.6.6.2/lib/olsrd_qaul`
    make
    ````
    # install the shared library
    ````
    sudo make install
    cd ../../../
    ````

Compile dynamic gateway plugin

    ````
    cd olsrd-0.6.6.2/lib/dyn_gw
    make
    ````
    # install the shared library
    ````
    sudo make install
    cd ../../../
    ````

Compile qaul CLI

    ````
    cd cli
    make OS=raspberry VoIP=no
    mkdir -p /home/pi/bin
    cp qaul /home/pi/bin
    cd ../
    ````


### Compile with VoIP

The qaul.net CLI binary can also be compiled with VoIP library included.
This does not make much sense, if there isn't anyone to pick up 
the incoming calls on the Rasberry. 

Compile olsrd, olsrd_qaul and the dynamic gateway plugin following 
the instructions above. Then follow this instructions here.


Compile pjsip library for VoIP

    # install required libraries
    ````
    sudo apt-get install build-essential automake autoconf libtool libasound2-dev libpulse-dev libssl-dev libsamplerate0-dev libcommoncpp2-dev libccrtp-dev libzrtpcpp-dev libdbus-1-dev libdbus-c++-dev libyaml-dev libpcre3-dev libgsm1-dev libspeex-dev libspeexdsp-dev libcelt-dev  libavformat-dev libavutil-dev libavcodec-dev libswscale-dev uuid-dev
    ````

    # compile pjsip
    ````
    cd pjproject-2.2.1
    ./configure
    ````

open the file pjlib/include/pj/config_site.h and enter

    #define PJ_IS_LITTLE_ENDIAN 1 
    #define PJ_IS_BIG_ENDIAN 0

compile pjsip

    ````
    make dep
    make
    cd ../
    ````

Compile qaul CLI

    ````
    cd cli
    make OS=raspberry
    mkdir -p /home/pi/bin
    cp qaul /home/pi/bin
    cd ../
    ````

