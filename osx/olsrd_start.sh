#!/bin/zsh

cd $1
#export DYLD_FRAMEWORK_PATH=$1
#echo $1/olsrd -i $2 $3 $4 -f $1/olsrd_osx.conf -d 0
$1/olsrd -i $2 $3 $4 -f $1/olsrd_osx.conf -d 0 -nofork
#$1/olsrd -i $2 $3 $4 -f $1/olsrd_osx.conf -d 0


