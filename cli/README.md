CLI (Command Line Interface) Client Instructions
================================================

The CLI client is for server and testing purposes.

Builds and runs on the following platforms:

* Linux
* OSX
* Windows 7/8
* Raspberry Pi

Feel free to port it to other platforms.


Compile Instructions
--------------------

Before building the CLI client executable you have to build the following 
libraries and executables:
* olsrd
* olsrd_qaul plugin 
  (you need to install the plugin to be found by the program)
* pjsip library
* qaullib

Check the build instructions for your platform 
* [ Windows ] (https://github.com/WachterJud/qaul.net/blob/master/win/README.md)
* [ OSX ]     (https://github.com/WachterJud/qaul.net/blob/master/osx/README.md)
* [ Linux ]   (https://github.com/WachterJud/qaul.net/blob/master/linux/README.md)
* [ Raspberry Pi ]   (https://github.com/WachterJud/qaul.net/blob/master/raspberry/README.md)


Build the CLI client

    cd cli
    # for Linux
    make OS=linux
    # for Windows (compile with MinGW)
    make OS=win
    # for OSX
    make OS=osx
    # for Raspberry Pi
    make OS=raspberry

Build the CLI client without pjsip (VoIP)

    cd cli
    # for Linux
    make OS=linux VoIP=no
    # for Windows (compile with MinGW)
    make OS=win VoIP=no
    # for OSX
    make OS=osx VoIP=no
    # for Raspberry Pi
    make OS=raspberry VoIP=no


Run CLI
-------

The following steps are needed to successfully run the CLI client

* Configure your wifi manually (make sure the IP in your DB):
  SSID: qaul.net, 
  Channel: 11, 
  IPv4: Manual IP configuration, 
  IP: 10.233.89.32, 
  Netmask: 255.0.0.0, 
  Gateway: 0.0.0.0 

* Start olsrd manually

    # linux
    sudo ./olsrd -i wlan0 -f ./olsrd.conf -d 1
    # osx
    sudo ./olsrd -i en1 -f ./olsrd.conf -d 1
    # windows 
    # run shell as administrator

* Run CLI client

    ./qaul

* Open the GUI in a web browser 
  http://localhost:8081/qaul.html


