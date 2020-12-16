#!/bin/sh -xe

cd emsdk
source ./emsdk_env.sh
cd ..

mkdir build || true
cd build

if [ "$1" = "release" ]; then
    BUILD_TYPE=RelWithDebInfo
else
    BUILD_TYPE=Debug
fi

emcmake cmake \
    -DEMSCRIPTEN="ON" \
    -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
    -DENABLE_OPENMP="OFF" \
    -DENABLE_LTO="OFF" \
    ..

emmake make