#!/bin/sh

for ((i=2; i<=64; i*=2))
do
  echo "Number of threads:" $i "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-="
  OMP_NUM_THREADS="$i" ./image
  echo
done
