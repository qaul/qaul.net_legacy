qaul.net Server
===============

The server is a passive participant in a qaul.net that provides interfaces to the outside
world. 

The server offers the following services:
* qaul2twitter: 
  Sends all messages from the qaul.net to twitter.
* twitter2qaul: 
  Sends messages to a twitter account to the qaul.net
* auto downloader: 
  Downloads all advertised files and stores it on the server.


Installation and Compile Instructions
--------------------------------------

* compile the qaul cli program without VoIP support

	cd cli
	# example for linux
	make OS=linux VoIP=no


* install the php web part as described in php/README.md
* for VPN connections of olsrd use the specific server/olsrd_vpn.conf
