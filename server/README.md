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

	````bash
	cd cli
	````
	# example for linux
	````bash
	make OS=linux VoIP=no
	````


* install the php web part as described in [php/README.md](https://github.com/WachterJud/qaul.net/tree/master/server/php)
* for VPN connections of olsrd use the specific [server/olsrd_vpn.conf](https://github.com/WachterJud/qaul.net/blob/master/server/olsrd_vpn.conf)
