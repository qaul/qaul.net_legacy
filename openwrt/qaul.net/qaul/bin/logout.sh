#!/bin/sh

## logout user

IFS="&="
set -- $HTTP_COOKIE
MD5STRING=$2

if [ "$MD5STRING" != "" ]
then
	rm /qaul/auth/$MD5STRING
fi

exit 0
