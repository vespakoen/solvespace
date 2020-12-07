#!/bin/sh -xe

sudo apt-get update -qq

sudo apt-get install -q -y \
  libx11-dev \
  mesa-common-dev \
  zlib1g-dev \
  libgl-dev \
  libglu-dev \
  libspnav-dev \
  libxi-dev \
  libxext-dev \
  gperf \
  automake \
  autopoint

git submodule update --init

./vcpkg/bootstrap-vcpkg.sh
./vcpkg/vcpkg install