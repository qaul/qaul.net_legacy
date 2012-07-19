Windows Instructions
====================

qaul.net works on:
* Windows 7


Installation and Compile Instructions
--------------------------------------

Compile pjproject with Visual Studio Express 2008
* download and install Visual Studio Express 2008
  http://www.microsoft.com/visualstudio/en-us/products/2008-editions/express
* download and install DirectX Software Development Kit 
  http://www.microsoft.com/en-us/download/confirmation.aspx?id=6812
* open the project in Visual Studio Express 2008 and hit F7 to compile


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
* download and install mingw
  http://sourceforge.net/projects/mingw/files/

Compile olsrd

    cd olsrd-0.6.0
    make

Compile olsrd_qaul plugin

    cd olsrd-0.6.0/lib/olsrd_qaul
    make

Compile libqaul.dll

    cd libqaul
    make win

    
