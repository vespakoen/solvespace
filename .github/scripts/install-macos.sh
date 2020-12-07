#!/bin/sh -xe

git submodule update --init
brew install gperf libomp automake

./vcpkg/bootstrap-vcpkg.sh
VCPKG_FEATURE_FLAGS=manifests ./vcpkg/vcpkg install