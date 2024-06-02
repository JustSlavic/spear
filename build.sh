#!/bin/bash

linux_debug_build() {
    CXX_FLAGS="-std=c++14 -g3"
    WARNINGS="-Wall -Werror"
    DEFINES="-DDEBUG=1 -DOS_LINUX=1"
    INCLUDES="-I../code"
    LIBS="-lX11 -lGL -lGLX -lXrandr"

    g++ $CXX_FLAGS $WARNINGS $INCLUDES $DEFINES ../code/main_linux.cpp -o spear $LIBS
}

linux_release_build() {
    CXX_FLAGS="-std=c++14"
    WARNINGS="-Wall -Werror"
    DEFINES="-DRELEASE=1 -DOS_LINUX=1"
    INCLUDES="-I../code"
    LIBS="-lX11 -lGL -lGLX"

    g++ $CXX_FLAGS $WARNINGS $INCLUDES $DEFINES ../code/main_linux.cpp -o spear $LIBS
}

macos_debug_build() {
    command=$1

    CXX_FLAGS="-std=c++17 -g -fno-rtti"
    WARNINGS="-Wall -Werror -Wno-unused-variable -Wno-unused-but-set-variable"
    DEFINES="-DDEBUG=1 -DOS_MAC=1"
    INCLUDES="-I../code -I../code/based -I/opt/homebrew/Cellar/sdl2/2.30.3/include"
    LIBS="-lSDL2 -L/opt/homebrew/Cellar/sdl2/2.30.3/lib -framework OpenGL"

    # clang++ $CXX_FLAGS $WARNINGS $INCLUDES $DEFINES ../code/main_sdl.cpp -o spear $LIBS -framework OpenGL

    clang++ $CXX_FLAGS $WARNINGS $INCLUDES $DEFINES ../code/main_sdl_opengl.cpp -o spear $LIBS -framework OpenGL
    # clang++ $CXX_FLAGS $WARNINGS ../code/main_sdl_vulkan.cpp -o vulkan -I/opt/homebrew/Cellar/sdl2/2.30.3/include -lSDL2 -L/opt/homebrew/Cellar/sdl2/2.30.3/lib -I/Users/vradko/VulkanSDK/1.3.283.0/macOS/include

    # swiftc ../code/main_macos.swift -o spear
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
        if [[ $? == 0 ]]; then
            ./tests
        fi
    else
        echo "Could not recognize the command provided (${command})"
    fi
elif [[ $os_name == "Darwin" ]]; then
    if [[ $command == "debug" ]]; then
        macos_debug_build
    elif [[ $command == "release" ]]; then
        macos_release_build
    elif [[ $command == "tests" ]]; then
        g++ ../code/math/g2_generator.cpp -o generator
        ./generator
        g++ ../code/math/g3_generator.cpp -o generator
        ./generator
        # cp g3_operators.cpp ../code/math/g3_operators.hpp

        tests_build
        if [[ $? == 0 ]]; then
            ./tests
        fi
    else
        echo "Could not recognize the command provided (${command})"
    fi
else
    echo "Could not recognize the system I'm on! (${os_name})"
    exit 1
fi
