#!/bin/bash
for t in 1 2 4 8 12 16 20 24 32
do
  for n in 1 10 100 1000 10000 100000 500000 1000000
  do
     clang++  -Xpreprocessor  -fopenmp  -I/usr/local/include -I/opt/homebrew/opt/libomp/include -L/usr/local/lib -L/opt/homebrew/opt/libomp/lib -lomp proj01.cpp  -DCSV=1 -DNUMT=$t -DNUMTRIALS=$n  -o proj01
    ./proj01
  done
done
