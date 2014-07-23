#!/bin/sh

## configure qaul.net with random IP's at startup

## global variables
MYIP="UNDEFINED"
MYNUM=0

## create a random IP
RANDIP ()
{
	RANDPART
	NUM1=$MYNUM
	RANDPART
	NUM2=$MYNUM
	RANDPART
	NUM3=$MYNUM
	
	MYIP="10.$NUM1.$NUM2.$NUM3"
}

## create a random number from 1-254
RANDPART () 
{
	# first position
	POS1=`head /dev/urandom | tr -dc "012" | head -c1`
	
	# second and third position
	if [ $POS1 -eq 2 ]
	then
		POS2=`head /dev/urandom | tr -dc "0-5" | head -c1`
		
		if [ $POS2 -eq 5 ]
		then
			POS3=`head /dev/urandom | tr -dc "0-4" | head -c1`
		else
			POS3=`head /dev/urandom | tr -dc "0-9" | head -c1`
		fi
	elif [ $POS1 -eq 0 ]
	then
		POS1=""
		POS2=`head /dev/urandom | tr -dc "0-9" | head -c1`
		
		if [ $POS2 -eq 0 ]
		then
			POS2=""
			POS3=`head /dev/urandom | tr -dc "1-9" | head -c1`
		else
			POS3=`head /dev/urandom | tr -dc "0-9" | head -c1`
		fi		
	else
		POS2=`head /dev/urandom | tr -dc "0-9" | head -c1`
		POS3=`head /dev/urandom | tr -dc "0-9" | head -c1`
	fi
	
	MYNUM="$POS1$POS2$POS3"
}

## execute on first startup
if [ -f /qaul/configured ]
then
	# nothing to be done
	exit 0
else
	echo "configure qaul IP"
	
	# configure ethernet
	RANDIP
	uci set network.qauleth.ipaddr=$MYIP

	# configure wifi
	RANDIP
	uci set network.qaulwifi.ipaddr=$MYIP

	# commit and restart
	uci commit network
	/etc/init.d/network restart
	/etc/init.d/olsrd restart

	# create configured file
	touch /qaul/configured
fi

exit 0
