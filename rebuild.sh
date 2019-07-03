#!/bin/sh

BUILD_DIR="build"

#if there is no build directory create one
if [ ! -d "$BUILD_DIR" ]; then
	mkdir "$BUILD_DIR"
fi

#enter build directory, erase content and make /sys/assets directory
cd "$BUILD_DIR"
rm -rf *

cmake -DCMAKE_BUILD_TYPE=Debug ../

