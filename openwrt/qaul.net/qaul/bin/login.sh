#!/bin/sh

## login user
## usage:
##     ./login.sh <PASSWORD> <SESSIONHASH> <IP>

# check if password is valid

# create session 
echo "$3" > "/qaul/auth/$2"

echo "1"

exit 0
