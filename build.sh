#!/bin/bash

CXX_FLAGS="-std=c++14"
LINKER_FLAGS=""
WARNINGS="-Wall -Werror" # // /W4 /WX /wd4201 /wd4100 /wd4189 /wd4505 /wd4702 /D_CRT_SECURE_NO_WARNINGS
DEFINES="-DDEBUG=1 -DOS_LINUX=1"
INCLUDES=-I../code
LIBS=-lX11

g++ $CXX_FLAGS $WARNINGS $INCLUDES ../code/main_linux.cpp -o spear $LIBS
