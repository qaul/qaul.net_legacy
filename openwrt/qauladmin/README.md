qaul.net admin helper
=====================

qaul.net admin executes administrative tasks such as checking the 
user authentication or the scripted web interface on the OpenWRT 
platform.


Compile
-------

To compile this binary you need to download and install the 
openwrt Toolchain and extract it in the openwrt directory.

    # build executable
    make


Install Manually
----------------

Copy the application in openwrt to /qaul/bin/qauladmin

    # replace ROUTERIP with the IP address of your openwrt router
    scp qauladmin root@ROUTERIP:/qaul/bin/
