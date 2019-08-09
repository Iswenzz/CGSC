#!/bin/bash

#Compiling: release
echo `gcc -m32 -Wall -O1 -s -fvisibility=hidden -mtune=prescott -c *.c`
#Linking
echo `gcc -m32 -s -shared -fvisibility=hidden -o gsclinq.so *.o`
#Cleaning up
echo `rm *.o`