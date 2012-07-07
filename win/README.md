Windows Instructions
====================

qaul.net works on:
* Windows 7


Installation and Compile Instructions
--------------------------------------

qaul.net is written in Visual C++ (managed code)

* Install Microsoft Visual Studio Express (the free version of visual studio)

Copy www folder into target folders:
    
    xcopy www win\Debug\www\ /e
    xcopy www win\Release\www\ /e


### Optional

To develop and compile olsrd, olsrd_qaul plugin and the dynamic libqaul.dll you need to install MinGW

Compile olsrd

    cd olsrd-0.6.0
    make

Compile olsrd_qaul plugin

    cd olsrd-0.6.0/lib/olsrd_qaul
    make

Compile libqaul.dll

    cd libqaul
    make win

    
