#!/bin/sh

## check user authentication
## based on REMOTE_ADDR and HTTP_COOKIE

IFS="&="
set -- $HTTP_COOKIE
MD5STRING=$2

iptest=`cat /qaul/auth/$MD5STRING`

if ["$iptest" = "$REMOTE_ADDR"]
then
	# check how old
	current=`date +%s`
	last_modified=`stat -c "%Y" $file`
	
	if [ $(($current-$last_modified)) -gt 600 ]; then 
		rm /qaul/auth/$MD5STRING
		echo "0"
	else 
		touch /qaul/auth/$MD5STRING
		echo "1"
	fi
else
	echo "0"
fi

exit 0
