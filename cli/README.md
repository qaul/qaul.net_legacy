CLI (Command Line Interface Client) Instructions
================================================

The CLI client is mainly for testing purposes.

builds and runs on the following platforms:
* Linux
* OSX
* Windows 7

Feel free to port it to the other supported platforms.


Compile Instructions
--------------------

Before building the CLI client executable you have to build the following 
libraries and executables:
* olsrd
* olsrd_qaul plugin
* pjsip library
* qaullib

Check the build instructions for your platform 
* [ Windows ] (https://github.com/WachterJud/qaul.net/blob/master/win/README.md)
* [ OSX ]     (https://github.com/WachterJud/qaul.net/blob/master/osx/README.md)
* [ Linux ]   (https://github.com/WachterJud/qaul.net/blob/master/linux/README.md)


Build the CLI client

    cd cli
    # for Linux
    make OS=linux
    # for Windows (compile with MinGW)
    make OS=win
    # for OSX
    make OS=osx


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
  http://localhost:8081/jqm_qaul.html


