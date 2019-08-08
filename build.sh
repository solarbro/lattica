#!/bin/bash

function build_local() {
    prepare_build_dir
    cd build/$1/
    make
    cd ../../../
}

function prepare_build_dir() {
    if [ ! -d "build" ]; then
        mkdir build
    fi
    cd build
    cmake .. -DCMAKE_INSTALL_PREFIX=$PWD/install
    cd ..
}

function printHeader() {
    echo ======================
    echo BUILDING $1
    echo ======================
}

if [ "$#" = 0 ]; then 
    echo "Target not specified."
    echo "Usage: ./build.sh target"
    exit 1
fi

if [ "$1" = "dependencies" ]; then
    # pull submodules
    git submodule update --init

    # generate custom cmake for dependencies
    cd dependencies
    mkdir -p build
    cd build
    cmake ..
    cd ../../

    # build the submodules
    # build imgui
    printHeader imgui
    cd dependencies/build/cmake_imgui/
    make
    cd ../../../

    # done
elif [ "$1" = "lib" ]; then
    build_local "src"
elif [ "$1" = "test" ]; then
    build_local "test"
elif [ "$1" = "clean" ]; then
    rm -rf build
elif [ "$1" = "run" ]; then
    cd build/test/
    ./viewer
    cd ../../
elif [ "$1" = "install" ]; then
    if [ ! "$#" = 2 ]; then
        echo "Invalid args"
        echo "Usage: ./build.sh $1 PROGRAM"
        exit 1
    fi
    cd build/src/$2
    make install
    cd ../../../
else
    echo "Invalid target" $1
    echo "BUILD usage: ./build.sh TARGET"
    echo "Where TARGET can be one of the following:"
    echo "  lib          - Build the lattica library"
    echo "  test         - Build the simple program for testing the library"
    echo "  dependencies - Update and build external libraries"
    echo "Other commands: "
    echo "run     - Run the test program"
    echo "          Usage: ./build.sh run"
    echo "clean   - Clean up build artefacts"
    echo "          Usage: ./build.sh clean"
    echo "install - Prepare the installation package for the specified target"
    echo "          Usage: ./build.sh TARGET"
fi