qaul.net Server
===============

The server is a passive participant in a qaul.net that provides interfaces to the outside
world.

Interfaces:
* qaul2twitter: 
  Sends all messages from the qaul.net to twitter.
* twitter2qaul: 
  Sends messages to a twitter account to the qaul.net
* auto downloader: 
  Downloads all advertised files and stores it on the server.


Installation and Compile Instructions
--------------------------------------

The install and compile instructions are for 

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


Web part
--------

Install the web Server

