#!/bin/bash
set -eo pipefail

TARGET_CPU=unix64
BUILD_DIR_DEBUG=build-$TARGET_CPU-debug
BUILD_DIR_RELEASE=build-$TARGET_CPU-release
GEN_CMD=gen-$TARGET_CPU.sh
CORES=-j$(grep -c ^processor /proc/cpuinfo)

echo Building and installing third party products...
#. ./$GEN_CMD
pushd $BUILD_DIR_DEBUG
make $CORES
popd
pushd $BUILD_DIR_RELEASE
make $CORES
popd
