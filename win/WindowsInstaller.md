Create Windows Installer
========================

This is how you create a .exe installer for Windows. 

* Build the executable for Release. 
* Copy all necessary files from win/Release to binary_release/qaul.net
  Don't copy the file qaullib.db as this file needs to be newly created by the application.
* Open the Inno Setup Script win/qaul.net_win_installer.iss with the Inno Setup Compiler. 
  (I'm using version 5.4.3 .)
* Click on the build button (Build > Compile) and the installer is created automagically.