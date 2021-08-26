#!/bin/bash

iterations=5
threads=1
max_threads=8

while [ $threads -le $max_threads ]; do
  iterations=5
  echo "executing sss with $threads thread for $iterations times" 
  while [ $iterations -gt 0 ]; do
    ./p2 0 $threads
    iterations=`expr $iterations - 1`
  done

  iterations=5
  echo "executing sps with $threads thread for $iterations times" 
  while [ $iterations -gt 0 ]; do
    ./p2 1 $threads
    iterations=`expr $iterations - 1`
  done

  iterations=5
  echo "executing ppp with $threads thread for $iterations times" 
  while [ $iterations -gt 0 ]; do
    ./p2 2 $threads
    iterations=`expr $iterations - 1`
  done

  threads=`expr $threads + 1`
done

