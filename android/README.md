Android Instructions
====================

It should work on all ARM android devices starting from Android 2.3 Gingerbread
(due to before unsupported pthread functions in pjsip). 
The Android device needs to be rooted.

qaul.net was tested on the following devices:

<table>
<tr><th>Device           </th><th>2.3</th><th>4.0</th><th>CM9</th></tr>
<tr><td>Samsung Galaxy S2</td><td> Y </td><td> N </td><td> N </td></tr>
<tr><td>HTC Sensation    </td><td> ? </td><td> Y </td><td> Y </td></tr>
</table>

Legend:
* Y = it works
* N = it doesn't work yet
* ? = it wasn't tested yet


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
    export API_LEVEL=9
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
    cp external/olsrd/olsrd res/raw/olsrd
    cp external/olsrd/lib/olsrd_qaul/olsrd_qaul.so.0.1 res/raw/olsrd_qaul_so_0_1
    cd ../

