Windows Instructions
====================

qaul.net works on:
* Windows 7
* Windows 8

qaul.net does not work on Windows XP.


Installation and Compile Instructions
--------------------------------------

qaul.net is written in Visual C++ (managed code)

* Install Microsoft Visual C++ Express (the free version of visual studio) 
  ( I worked with Visual C++ Express 2010 )

Open Visual Studio and run qaul.net

### Optional

To develop and compile qaullib, pjsip, olsrd and olsrd_qaul plugin you will need to install MinGW
* download and install mingw
  http://sourceforge.net/projects/mingw/files/

Compile pjsip library for VoIP with mingw
(at the end of compilation, pjlib-test will throw an error, ignore that error)
    
	cd pjproject-2.0.1
	./aconfigure --disable-ssl CFLAGS='-m32' LDFLAGS='-m32'
	make dep
    make
	cd ../

if make dep gives you - #error "PJ_HAS_HIGH_RES_TIMER is not defined!" - errors, download the pjproject as a zip from
the web and extract it again - this is a known error http://trac.pjsip.org/repos/wiki/FAQ#mingw-configure

Compile libqaul.dll with MinGw

    cd libqaul
    make OS=win
    cd ../

Compile olsrd

    cd olsrd-0.6.3
    make
	cp olsrd.exe ../win/Debug/
	cp olsrd.exe ../win/Release/
	cd ../

Compile olsrd_qaul plugin

    cd olsrd-0.6.3/lib/olsrd_qaul
    make
	cp olsrd_qaul.dll ../../../win/Debug/
	cp olsrd_qaul.dll ../../../win/Release/
	cd ../../../
