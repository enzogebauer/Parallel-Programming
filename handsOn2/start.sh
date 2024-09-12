#!/bin/sh

for ((i=1; i<=8; i++))
do
  echo "Number of threads:" $i "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-="  
  for ((j=1000; j<=80000; j+=1200))
  do
    OMP_NUM_THREADS="$i" ./integral "$j"
    echo "Number of threads:" $i "Number of intervals:" $j
  done
  echo 
done
