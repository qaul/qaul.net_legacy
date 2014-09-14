#!/bin/sh

##
# test internet sharing configuration input.
# this script has to be invoked from localhost without arguments.
# 
#    ./config_internet.sh
#

## test config_internet_loading
curl http://127.0.0.1:8081/config_internet_loading

## test config_internet_get
curl http://127.0.0.1:8081/config_internet_get


## test config_internet_set
# correct
curl --data "share=1&if=en0&e=1" http://127.0.0.1:8081/config_internet_set
curl --data "share=0&if=en0&e=1" http://127.0.0.1:8081/config_internet_set
# no, wrong & oversized values
curl --data "share=1" http://127.0.0.1:8081/config_internet_set
curl --data "e=1" http://127.0.0.1:8081/config_internet_set
curl --data "share=abc&if=\\';|;\';" http://127.0.0.1:8081/config_internet_set
curl --data "share=1&space=789932999237487282978993299923748728297899329992374872829789932999237487282978993299923748728297899329992374872829789932999237487282978993299923748728297899329992374872829789932999237487282978993299923748728297899329992374872829789932999237487282978993299923748728297899329992374872829789932999237487282978993299923748728297899329992374872829&e=1" http://127.0.0.1:8081/config_internet_set
