#!/bin/bash

iterations=5
threads=1
max_threads=8

while [ $threads -le $max_threads ]; do
  time ./run_single.sh 0 $threads
  echo "executing sss with $threads thread for $iterations times"
  echo "--------------------------------------------------------"

  time ./run_single.sh 1 $threads
  echo "executing sps with $threads thread for $iterations times"
  echo "--------------------------------------------------------"

  time ./run_single.sh 2 $threads
  echo "executing ppp with $threads thread for $iterations times"
  echo "--------------------------------------------------------"
  
  time ./run_single.sh 3 $threads
  echo "executing ssp with $threads thread for $iterations times"
  echo "--------------------------------------------------------"

  time ./run_single.sh 4 $threads
  echo "executing spp with $threads thread for $iterations times"
  echo "--------------------------------------------------------"

  time ./run_single.sh 5 $threads
  echo "executing pss with $threads thread for $iterations times"
  echo "--------------------------------------------------------"
  
  time ./run_single.sh 6 $threads
  echo "executing psp with $threads thread for $iterations times"
  echo "--------------------------------------------------------"

  time ./run_single.sh 7 $threads
  echo "executing pps with $threads thread for $iterations times"
  echo "--------------------------------------------------------"

  threads=`expr $threads + 1`
done

