qaul.net cli on Server
======================

How to compile the qaul cli application on a Server without audio card.

This Software has been run successfully on 
* Ubuntu Linux 64bit
* OSX 10.8


Installation and Compile Instructions
--------------------------------------

Compile pjsip library for Server, without any sound or video

    # install needed packages
    sudo apt-get install gcc g++
    # compile pjsip
    cd pjproject-2.2.1
    ./configure --disable-ssl --disable-sound --disable-video --disable-oss --enable-ext-sound --disable-ffmpeg --disable-v4l2
    make dep
    make
    cd ../

Compile qaul.net cli executable

    cd server/cli
    make OS=linux


