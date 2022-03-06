#!/bin/bash
current_path='dirname "$0"'
src=$current_path/source/c++
g++ -c -O3 -std=gnu++14 -o src/pointCheap.o  src/pointCheap.cpp -fopenmp
g++ -c -O3 -std=gnu++14 -o src/interpolation2Large.o src/interpolation2Large.cpp -fopenmp
g++ -c -O3 -std=gnu++14 -o src/biotSavartGauge.o src/biotSavartGauge.cpp -fopenmp
g++ -c -O3 -std=gnu++14 -std=gnu++14 -o src/mainObservationalWindingPotentialFast.o src/mainObservationalWindingPotentialFast.cpp -fopenmp
g++ -O3 -std=gnu++14 -o observationalWindingPotentialFast src/pointCheap.o src/interpolation2Large.o  src/biotSavartGauge.o src/mainObservationalWindingPotentialFast.o -fopenmp
