#!/bin/bash

linux_debug_build() {
    CXX_FLAGS="-std=c++14 -g3"
    WARNINGS="-Wall -Werror"
    DEFINES="-DDEBUG=1 -DOS_LINUX=1"
    INCLUDES="-I../code"
    LIBS="-lX11 -lGL -lGLX"

    g++ $CXX_FLAGS $WARNINGS $INCLUDES $DEFINES ../code/main_linux.cpp -o spear $LIBS
}

linux_release_build() {
    CXX_flAgs="-std=c++14"
    WARNINGS="-Wall -Werror"
    DEFINES="-DRELEASE=1 -DOS_LINUX=1"
    INCLUDES="-I../code"
    LIBS="-lX11 -lGL -lGLX"

    g++ $CXX_FLAGS $WARNINGS $INCLUDES $DEFINES ../code/main_linux.cpp -o spear $LIBS
}

macos_debug_build() {
    command=$1

    swiftc ../code/main_macos.swift -o spear
}

macos_release_build() {
    echo "Not ready yet!"
}

tests_build() {
    CXX_FLAGS="-std=c++14 -g3"
    WARNINGS="-Wall -Werror"
    DEFINES="-DDEBUG=1 -DOS_LINUX=1"
    INCLUDES="-I../code"
    LIBS=""

    g++ $CXX_FLAGS $WARNINGS $INCLUDES $DEFINES ../code/tests/tests.cpp -o tests $LIBS
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

command="debug"

if [[ $1 != "" ]]; then
    command="$1"
fi

move_to_build_dir

os_name=$(uname -s)

if [[ $os_name == "Linux" ]]; then
    if [[ $command == "debug" ]]; then
        linux_debug_build
    elif [[ $command == "release" ]]; then
        linux_release_build
    elif [[ $command == "tests" ]]; then
        tests_build
    fi
elif [[ $os_name == "Darwin" ]]; then
    if [[ $command == "debug" ]]; then
        macos_debug_build
    elif [[ $command == "release" ]]; then
        macos_release_build
    elif [[ $command == "tests" ]]; then
        tests_build
        ./tests
    fi

else
    echo "Could not recognize the system I'm on! (${os_name})"
    exit 1
fi

cd ../
