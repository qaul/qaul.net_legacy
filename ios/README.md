iOS Instructions
================

qaul.net tested on:
* iOS 5.1.1 iphone 4


Installation and Compile Instructions
--------------------------------------

Open the qaullib Xcode project ios/qaullib.xcodeproj and build the static library.

Open the ios project in Xcode to run qaul.net


optional:

Compile olsrd

    cd olsrd-0.6.0
    make
    cp olsrd ../ios/
    cd ../

Compile olsrd_qaul plugin

    cd olsrd-0.6.0/lib/olsrd_qaul
    make
    cp olsrd_qaul.so.0.1 ../../../ios/
    cd ../../../
