Presentation Screen
===================

Fullscreen version for Windows7. This version runs as a kiosk system for presentations of 
this software. The user can not exit the qaul.net application. Some features have been
deactivated.


Set Up
------

Screen configuration

* 24" screen with full HD resolution
* Set screen to portrait mode
* Set screen resolution to: 1920 x 1200

Install and prepare Firefox for the GUI

* Install firefox
* Configure start up URL to http://localhost:8081/jqm_presentation.html
* Install the firefox addon "Default FullZoom Level" 
  https://addons.mozilla.org/en-US/firefox/addon/default-fullzoom-level/
* Configure "Default FullZoom Level" addon to zoom level 250%
* Install the firefox addon "status-4-evar" to remove the link target pop up
* Install the firefox addon "R-Kiosk"

To make further configuration changes to firefox one has to deactivate the R-Kiosk addon.
Therefore one has to start firefox in safe mode.

    "C:\Program Files (x86)\Mozilla Firefox\firefox.exe" -safe-mode


Start Up
--------

1) Start qaul.net
2) Start firefox


Shut down
---------

1) Close the firefox window with "Alt + F4"
2) Close the qaul.net application


Changes Compared to the Master Branch
-------------------------------------

* User can not open files.
* Configuration tab removed.
* Interface is rendered full screen in firefox Kiosk mode and 200% zoomed
