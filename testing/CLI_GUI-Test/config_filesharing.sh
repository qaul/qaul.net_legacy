#!/bin/sh

##
# test file sharing configuration input.
# this script has to be invoked from localhost without arguments.
# 
#    ./config_filesharing.sh
#

## test config_files_get
curl http://127.0.0.1:8081/config_files_get


## test config_files_set
# correct
curl --data "download=1&space=4800000&size=50000&e=1" http://127.0.0.1:8081/config_files_set
curl --data "download=0&space=0&size=0&e=1" http://127.0.0.1:8081/config_files_set
# no, wrong & oversized values
curl --data "download=1" http://127.0.0.1:8081/config_files_set
curl --data "e=1" http://127.0.0.1:8081/config_files_set
curl --data "download=abc&space=9ole&size=\\';|;\';" http://127.0.0.1:8081/config_files_set
curl --data "download=1.24&space=2.34&size=13.69584&e=1" http://127.0.0.1:8081/config_files_set
curl --data "download=7899329992374872829&space=7899329992374872829&size=7899329992374872829&e=1" http://127.0.0.1:8081/config_files_set
curl --data "download=1899329992374872829&space=7899329992374872829&size=7899329992374872829&e=1" http://127.0.0.1:8081/config_files_set
