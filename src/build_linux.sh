#!/bin/bash
g++ -Wall -fPIC -c test.cpp
g++ -shared -Wl,-soname,test.so -o test.so *.o
rm *.o
