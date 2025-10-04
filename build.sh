#!/bin/bash

set -e

linux_debug_build() {
    CXX_FLAGS="-std=c++14 -g3"
    WARNINGS="-Wall -Werror"
    DEFINES="-DDEBUG=1 -DOS_LINUX=1"
    INCLUDES="-I../code -I../code/engine"
    LIBS="-lX11 -lGL -lGLX -lXrandr"

    g++ $CXX_FLAGS $WARNINGS $INCLUDES $DEFINES ../code/main_linux.cpp -o spear $LIBS
}

linux_release_build() {
    CXX_FLAGS="-std=c++14"
    WARNINGS="-Wall -Werror"
    DEFINES="-DRELEASE=1 -DOS_LINUX=1"
    INCLUDES="-I../code -I../code/engine"
    LIBS="-lX11 -lGL -lGLX"

    g++ $CXX_FLAGS $WARNINGS $INCLUDES $DEFINES ../code/main_linux.cpp -o spear $LIBS
}

macos_build_engine() {
    compiler_options=$1
    compiler_output="spear_engine"

    compiler_command="clang $compiler_options -o bin/$compiler_output code/engine/main_sdl_opengl.c"
    echo "$compiler_command"
    exec $($compiler_command)
}

macos_build_game() {
    compiler_options=$1
    compiler_output="spear_game.so"

    compiler_command="clang $compiler_options -shared -undefined dynamic_lookup -o bin/$compiler_output code/game_rpg/game.c"
    echo "$compiler_command"
    exec $($compiler_command)
}

macos_vulkan_build_shader() {
    shader_name=$1
    vert_shader_input="code/engine/$1.vs.glsl"
    frag_shader_input="code/engine/$1.fs.glsl"

    vert_shader_output="bin/$1.vs.spv"
    frag_shader_output="bin/$1.fs.spv"

    cmd="$VULKAN_SDK/bin/glslc -fshader-stage=vertex -o $vert_shader_output $vert_shader_input"
    echo "$cmd"
    exec $($cmd)

    cmd="$VULKAN_SDK/bin/glslc -fshader-stage=fragment -o $frag_shader_output $frag_shader_input"
    echo "$cmd"
    exec $($cmd)
}

macos_vulkan_build() {
    VULKAN_SDK="/Users/vradko/VulkanSDK/1.3.283.0/macOS"
    INCLUDES_VULKAN="-I$VULKAN_SDK/include"
    LIBS_VULKAN="-lSDL2 -lvulkan -L/opt/homebrew/Cellar/sdl2/2.30.3/lib -L$VULKAN_SDK/lib"
    RPATH_OPTION="-rpath $VULKAN_SDK/lib"
    cmd="clang $C_FLAGS $WARNINGS $DEFINES $INCLUDES $INCLUDES_VULKAN $LIBS_VULKAN $RPATH_OPTION -o bin/spear_engine_vulkan code/engine/main_sdl_vulkan.c"
    echo "$cmd"
    exec $($cmd)

    macos_vulkan_build_shader example
}

macos_debug_build() {
    C_FLAGS="-std=c89 -g"
    WARNINGS="-Wall -Werror -Wno-comment -Wno-unused-function"
    DEFINES="-DDEBUG=1 -DDLL_BUILD=0"
    INCLUDES="-Icode -I/opt/homebrew/Cellar/sdl2/2.30.3/include"
    LIBS="-lSDL2 -L/opt/homebrew/Cellar/sdl2/2.30.3/lib -framework OpenGL"

    macos_build_engine "$C_FLAGS $WARNINGS $DEFINES $INCLUDES $LIBS"
    # macos_build_game   "$C_FLAGS $WARNINGS $DEFINES $INCLUDES $LIBS"

    # macos_vulkan_build

    # clang -std=c89 -g -Wall -Werror -Wno-comment -Icode -o ./bin/run_ecs code/main_ecs.c
}

macos_release_build() {
    C_FLAGS="-std=c89 -O2"
    WARNINGS="-Wall -Werror -Wno-comment"
    DEFINES="-DDLL_BUILD=1"
    INCLUDES="-Icode -I/opt/homebrew/Cellar/sdl2/2.30.3/include"
    LIBS="-lSDL2 -L/opt/homebrew/Cellar/sdl2/2.30.3/lib -framework OpenGL"

    macos_build_engine "$C_FLAGS $WARNINGS $DEFINES $INCLUDES $LIBS"
    macos_build_game   "$C_FLAGS $WARNINGS $DEFINES $INCLUDES $LIBS"
}

# -----------------------------------------------

os_name=$(uname -s)
command="debug"
if [[ $1 != "" ]]; then
    command="$1"
fi

mkdir -p bin

if [[ $os_name == "Linux" ]]; then
    if [[ $command == "debug" ]]; then
        linux_debug_build
    elif [[ $command == "release" ]]; then
        linux_release_build
    else
        echo "Could not recognize the command provided (${command})"
    fi
elif [[ $os_name == "Darwin" ]]; then
    if [[ $command == "debug" ]]; then
        macos_debug_build
    elif [[ $command == "release" ]]; then
        macos_release_build
    else
        echo "Could not recognize the command provided (${command})"
    fi
else
    echo "Could not recognize the system I'm on! (${os_name})"
    exit 1
fi
