Android Instructions
====================

qaul.net was tested on:
* Android 2.3 Gingerbread

It should work on all ARM android devices starting from Android 2.2 Froyo. 
The Android device needs to be rooted.


Installation and Compile Instructions
--------------------------------------

Requirements:
* Install the Android SDK on your computer.
* Download and Install NDK on your computer to compile qaullib.
* Install the Android IDE in Eclipse.

Open the android project in Eclipse and run it on your Smartphone.

### Optional

Compile pjsip library for VoIP 
(at the end of compilation, pjlib-test will throw an error, ignore that error)

    cd pjproject_android
    export ANDROID_NDK=/path_to_android_ndk_dir
    export API_LEVEL=8
    ./configure-android
    make dep
    make
    cd ../

Compile qaullib & tether library

    cd android
    ndk-build
    cd ../

Compile olsrd & olsrd_qaul plugin

    cd android
    make -C external
    cp external/olsrd/lib/olsrd_qaul/olsrd_qaul.so.0.1 res/raw/olsrd_qaul_so_0_1
    cd ../

