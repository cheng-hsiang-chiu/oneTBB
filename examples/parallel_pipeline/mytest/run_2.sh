#!/bin/bash

iterations=5
threads=1
max_threads=8
tokens=128

while [ $threads -le $max_threads ]; do
  >&2 echo "executing sss with $threads threads and $tokens tokens for $iterations times"
  time ./run_single.sh 0 $threads $tokens
  echo "executing sss with $threads threads and $tokens tokens for $iterations times"
  echo "----------------------------------------------------------------------------"
  >&2 echo "----------------------------------------------------------------------------"

  >&2 echo "executing sps with $threads threads and $tokens tokens for $iterations times"
  time ./run_single.sh 1 $threads $tokens
  echo "executing sps with $threads threads and $tokens tokens for $iterations times"
  echo "----------------------------------------------------------------------------"
  >&2 echo "----------------------------------------------------------------------------"

  >&2 echo "executing ppp with $threads threads and $tokens tokens for $iterations times"
  time ./run_single.sh 2 $threads $tokens
  echo "executing ppp with $threads threads and $tokens tokens for $iterations times"
  echo "----------------------------------------------------------------------------"
  >&2 echo "----------------------------------------------------------------------------"
  
  >&2 echo "executing ssp with $threads threads and $tokens tokens for $iterations times"
  time ./run_single.sh 3 $threads $tokens
  echo "executing ssp with $threads threads and $tokens tokens for $iterations times"
  echo "----------------------------------------------------------------------------"
  >&2 echo "----------------------------------------------------------------------------"

  >&2 echo "executing spp with $threads threads and $tokens tokens for $iterations times"
  time ./run_single.sh 4 $threads $tokens
  echo "executing spp with $threads threads and $tokens tokens for $iterations times"
  echo "----------------------------------------------------------------------------"
  >&2 echo "----------------------------------------------------------------------------"

  >&2 echo "executing pss with $threads threads and $tokens tokens for $iterations times"
  time ./run_single.sh 5 $threads $tokens
  echo "executing pss with $threads threads and $tokens tokens for $iterations times"
  echo "----------------------------------------------------------------------------"
  >&2 echo "----------------------------------------------------------------------------"
  
  >&2 echo "executing psp with $threads threads and $tokens tokens for $iterations times"
  time ./run_single.sh 6 $threads $tokens
  echo "executing psp with $threads threads and $tokens tokens for $iterations times"
  echo "----------------------------------------------------------------------------"
  >&2 echo "----------------------------------------------------------------------------"

  >&2 echo "executing pps with $threads threads and $tokens tokens for $iterations times"
  time ./run_single.sh 7 $threads $tokens
  echo "executing pps with $threads threads and $tokens tokens for $iterations times"
  echo "----------------------------------------------------------------------------"
  >&2 echo "----------------------------------------------------------------------------"

  threads=`expr $threads + 1`
done

