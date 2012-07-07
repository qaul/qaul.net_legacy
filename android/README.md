Android Instructions
====================

qaul.net was tested on:
* Android 2.3 Gingerbread

It should work on all ARM android devices starting from Android 2.2 Froyo


Installation and Compile Instructions
--------------------------------------

Install the Android IDE in Eclipse to
Run the 
The Android device needs to be rooted.

### Optional

Compile qaullib & tether library

    ndk-build

Compile olsrd & olsrd_qaul plugin

    make -C external
    cp external/olsrd/lib/olsrd_qaul/olsrd_qaul.so.0.1 res/raw/olsrd_qaul_so_0_1