Linux Instructions
==================

qaul.net has been tested on:
* Ubuntu 12.04: Precise Pangolin, 32-Bit & 64-Bit
* Ubuntu 14.04: Precise Pangolin, 32-Bit & 64-Bit
* Debian 7: Wheezy, 64-Bit

It should run on all recent Debian & Ubuntu distributions.


Installation and Compile Instructions
--------------------------------------

Compile olsrd

    # install required package
    sudo apt-get install bison flex
    # compile olsrd
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

	# install required libraries
	sudo apt-get install g++
	# compile pjsip
    cd pjproject-2.2.1
    ./configure --disable-ffmpeg
    make dep
    make
    cd ../

Compile qaul.net

    # install gtk develper libraries
    sudo apt-get install libgtk-3-dev libwebkit-dev
    # change into directory and compile
    cd linux/qaul_gtk
    make
    cd ../../

Compile qaulhelper

    cd linux/qaulhelper_gtk
    make
    cd ../../



Troubleshooting
---------------

On Ubuntu 12.04 64-Bit pjsip failed compiling because it could't find the 
following program and library: cc1plus, libstdc++
They had to be linked manually to the standard directories:

    # link executable cc1plus
    sudo ln -s /usr/lib/gcc/x86_64-linux-gnu/4.6/cc1plus /usr/bin/cc1plus
    # link library stdc++
    sudo ln -s /usr/lib/gcc/x86_64-linux-gnu/4.6/libstdc++.a /usr/lib/libstdc++.a
