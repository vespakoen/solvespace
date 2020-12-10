#!/bin/sh -xe

if [ "$1" == "silicon" ]; then
    export ARCHFLAGS='-arch arm64'
    brew install --verbose --build-from-source libomp zlib cairo pixman
    brew reinstall --verbose --build-from-source --HEAD libpng freetype
else
    brew install libomp libpng zlib freetype
fi
git submodule update --init
