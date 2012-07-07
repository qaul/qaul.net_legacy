#!/bin/zsh

$1/socat UDP4-RECVFROM:53,fork UDP4-SENDTO:localhost:8053 &
$1/socat UDP4-RECVFROM:67,fork UDP4-SENDTO:localhost:8067 &
