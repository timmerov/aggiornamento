#!/bin/bash

BUILD_DIR=build-unix64
CORES=-j$(grep -c ^processor /proc/cpuinfo)
if [ -z ${CMAKE_EXE+X} ]; then
	CMAKE_EXE=cmake
fi

options="-DPLATFORM_FILE:STRING=unix64"

# check if we're running cygwin
if [ "$(expr substr $(uname -s) 1 6)" == "CYGWIN" ]; then
	options="$options -DCMAKE_LEGACY_CYGWIN_WIN32:STRING=0"
fi

mkdir -p $BUILD_DIR-debug
pushd $BUILD_DIR-debug
echo Build is unix64 debug.
$CMAKE_EXE \
	-G "Unix Makefiles" \
	-DCMAKE_BUILD_TYPE:STRING=Debug \
	$options \
	..
popd

mkdir -p $BUILD_DIR-release
pushd $BUILD_DIR-release
echo Build is unix64 release.
$CMAKE_EXE \
        -G "Unix Makefiles" \
	-DCMAKE_BUILD_TYPE:STRING=Release \
	$options \
	..
popd

echo
echo Unix makefiles are in build-unix64-debug and build-unix64-release.
