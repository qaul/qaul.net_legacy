#!/bin/sh

## logout user

IFS="&="
set -- $HTTP_COOKIE
MD5STRING=$2

rm /qaul/auth/$MD5STRING

exit 0
