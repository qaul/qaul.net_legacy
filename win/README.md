Windows Instructions
====================

qaul.net works on:
* Windows 7


Installation and Compile Instructions
--------------------------------------

Compile pjproject with MinGW

    cd pjproject-2.0.1
    ./configure --enable-shared --disable-static --enable-memalign-hack
    make 
    cd ..

Compile libqaul.dll with Windows Power shell

    cd libqaul
    make win
    cd ..

qaul.net is written in Visual C++ (managed code)

* Install Microsoft Visual C++ Express (the free version of visual studio) 
  ( I worked with Visual C++ Express 2010 )

Copy www folder into target folders:
    
    xcopy www win\Debug\www\ /e
    xcopy www win\Release\www\ /e

Open Visual Studio and run qaul.net

### Optional

To develop and compile olsrd, olsrd_qaul plugin you will need to install MinGW

Compile olsrd

    cd olsrd-0.6.0
    make

Compile olsrd_qaul plugin

    cd olsrd-0.6.0/lib/olsrd_qaul
    make

Compile libqaul.dll

    cd libqaul
    make win

    
