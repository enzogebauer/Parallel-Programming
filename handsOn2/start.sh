#!/bin/sh

for ((i=1; i<=8; i++))
do
  echo "Number of threads:" $i "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-="  
  OMP_NUM_THREADS="$i" ./count_primes
  echo 
done
