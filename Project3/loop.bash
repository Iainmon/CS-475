#!/bin/bash
for n in 1024 2048 4096 8192 16384 32768
do
  for m in true true true true true false false false false false false false false false false
  do
        clang++  -Xpreprocessor  -fopenmp  -I/usr/local/include -I/opt/homebrew/opt/libomp/include -L/usr/local/lib -L/opt/homebrew/opt/libomp/lib -lomp mutex03.cpp  -DNUMN=$n -DUSE_MUTEX=$m  -o mutex03
      ./mutex03
  done
done