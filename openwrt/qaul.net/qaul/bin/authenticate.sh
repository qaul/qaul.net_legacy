#!/bin/sh

## check user authentication
## based on REMOTE_ADDR and HTTP_COOKIE

IFS="&="
set -- $HTTP_COOKIE
MD5STRING=$2
AUTHFILE=/qaul/auth/$MD5STRING

if [ "$MD5STRING" != "" ]
then
	iptest=`cat $AUTHFILE`
	if [ "$iptest" = "$REMOTE_ADDR" ]
	then
		# check how old
		current=`date +%s`
		last_modified=`stat -c "%Y" $AUTHFILE`
		
		if [ $(($current-$last_modified)) -gt 600 ]
		then 
			rm $AUTHFILE 
			echo "0"
		else 
			touch $AUTHFILE
			echo "1"
		fi
	else
		echo "0"
	fi
else
	echo "0"
fi

exit 0
