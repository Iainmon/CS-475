#!/bin/bash

for i in 1 4
do
    echo ""
    echo NUMT = $i
    clang++ -DNUMT=$i -Xpreprocessor  -fopenmp  -I/usr/local/include  -L/usr/local/lib  -lomp main.cpp  -o  main
    ./main
done