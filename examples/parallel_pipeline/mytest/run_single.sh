#!/bin/bash

max_threads=8

pid=$1
threads=$2
tokens=$3

patterns=( 'sss' 'sps' 'ppp' 'ssp' 'spp' 'pss' 'psp' 'pps' )

iterations=5

echo "executing ${patterns[$pid]} with $threads thread for $iterations times" 
while [ $iterations -gt 0 ]; do
  ./p2 $pid $threads $tokens
  iterations=`expr $iterations - 1`
done

