OSX Instructions
================

qaul.net works on:
* OSX 1.5 Leopard (needs special build)
* OSX 1.6 Snow Leopard
* OSX 1.7 Lion
* OSX 1.8 Mountain Lion


Installation and Compile Instructions
--------------------------------------

Compile pjsip library for VoIP

    cd pjproject-2.1.0
    ./aconfigure --disable-ssl --disable-ffmpeg CFLAGS='-Wno-unused-label -m32' LDFLAGS='-m32'
    make dep
    make
    cd ../

Compile static qaullib. 
You may need to fix the file pjproject-2.0.1/pjlib/include/pj/compat/os_darwinos.h manually.
Change line 98

    typedef int socklen_t;
    
to
   
    #ifndef _SYS_SOCKET_H_
    typedef int socklen_t;
    #endif

Now you should be able to successfully compile libqaul   

    cd libqaul
    make OS=osx
    cd ../

Open the OSX project in Xcode to run qaul.net


### Optional

Compile olsrd

    cd olsrd-0.6.0
    make
    cp olsrd ../osx/
    cd ../

Compile olsrd_qaul plugin

    cd olsrd-0.6.0/lib/olsrd_qaul
    make
    cp olsrd_qaul.so.0.1 ../../../osx/
    # you need to install the shared library in /usr/lib/
    sudo make install
    cd ../../../


Installer
---------

To build the qaul.net package installer you need to download and install the free and
open source program "packages":
http://s.sudre.free.fr/Software/Packages/about.html

Put the applications (qaul.app, qaulhelper, olsrd, olsrd plugins, olsrd configuration 
files, socat) to their locations:

    /Applications
        /qaul.app
    /Library
        /qaul.net
            /olsrd
            /olsrd_dyn_gw.so.0.5
            /olsrd_osx.conf
            /olsrd_osx_gw.conf
            /olsrd_qaul.so.0.1
            /qaulhelper
            /socat

Open the project file osx/installer/qaul.net with "packages" and build the installer 
by selecting "Build > Build" from the menu.
