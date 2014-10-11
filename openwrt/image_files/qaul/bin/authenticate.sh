#!/bin/sh

## check user authentication
## based on REMOTE_ADDR and HTTP_COOKIE

IFS="&="
set -- $HTTP_COOKIE
MD5STRING=`echo "$2" | sed -E "s/^([^ ;\|\/\$\"]*)|(.*)$/\1/"`

if [ "$MD5STRING" != "" ]
then
	AUTHFILE=/qaul/auth/$MD5STRING
	iptest=`cat $AUTHFILE`
	
	if [ "$iptest" = "$REMOTE_ADDR" ]
	then
		# check how old
		current=`date +%s`
		last_modified=`stat -t $AUTHFILE | sed -E 's/^[^ ]+ [^ ]+ [^ ]+ [^ ]+ [^ ]+ [^ ]+ [^ ]+ [^ ]+ [^ ]+ [^ ]+ [^ ]+ ([0-9]+) .*$/\1/'`
		
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
