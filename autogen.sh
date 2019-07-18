#!/bin/sh

BUILD_DIR="build"

#if there is no build directory create one
if [ ! -d "$BUILD_DIR" ]; then
	echo "Please build the project first."
	exit 1
fi

if [ "$1" != "" ]; then
	echo "Scanning folder: $1"
	for entry in "$1"/*.png
	do
		echo "$entry"
		./build/vimgconvert $entry 15 Y
	done
else
	echo "No folder provided"
fi
