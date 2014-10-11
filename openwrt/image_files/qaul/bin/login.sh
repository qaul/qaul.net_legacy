#!/bin/sh

## login user
## usage:
##     ./login.sh <PASSWORD> <SESSIONHASH> <IP>

# get salt from shadow file
SALT=`grep ^root: /etc/shadow | sed -E "s/^root:([^:]+).+$/\1/" | sed -E "s/[\$]/\\\\$/g"`

# check if password is valid
login=`/qaul/bin/qauladmin login "$1" "$SALT"`

if [ "$login" = "success" ]
then
	# create session 
	echo "$3" > "/qaul/auth/$2"
	echo "1"
else
	echo "0"
fi

exit 0
