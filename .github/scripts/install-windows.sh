#!/bin/sh -xe

git submodule update --init

./vcpkg/bootstrap-vcpkg.bat
./vcpkg/vcpkg install