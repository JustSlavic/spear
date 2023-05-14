#!/bin/bash

linux_build() {
    CXX_FLAGS="-std=c++14 -g3"
    LINKER_FLAGS=""
    WARNINGS="-Wall -Werror" # // /W4 /WX /wd4201 /wd4100 /wd4189 /wd4505 /wd4702 /D_CRT_SECURE_NO_WARNINGS
    DEFINES="-DDEBUG=1 -DOS_LINUX=1"
    INCLUDES="-I../code"
    LIBS="-lX11 -lGL -lGLX"

    g++ $CXX_FLAGS $WARNINGS $INCLUDES $DEFINES ../code/main_linux.cpp -o spear $LIBS

    cd ../
}

macos_build() {
    swiftc ../code/main_macos.swift -o spear
}

move_to_build_dir() {
    path=$(pwd)
    dir=$(basename "$path")

    if [[ "$dir" != "build" ]]; then
        if [[ ! -d "build" ]]; then
            mkdir build
            cd build
        else
            cd build
        fi
    fi
}

# -----------------------------------------------

move_to_build_dir

os_name=$(uname -s)

if [[ $os_name == "Linux" ]]; then
    linux_build
elif [[ $os_name == "Darwin" ]]; then
    macos_build
else
    echo "Could not recognize the system I'm on! (${os_name})"
    exit 1
fi
