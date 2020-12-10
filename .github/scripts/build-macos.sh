#!/bin/sh -xe

mkdir build || true
cd build

ENABLE_CLI="ON"
ENABLE_TESTS="ON"
if [ "$1" = "release" ]; then
    BUILD_TYPE="RelWithDebInfo"
    ENABLE_SANITIZERS="OFF"
    ENABLE_LTO="ON"
else
    BUILD_TYPE="Debug"
    ENABLE_SANITIZERS="OFF"
    ENABLE_LTO="OFF"
fi

OSX_ARCH="x86_64"
if [ "$2" = "silicon" ]; then
    OSX_ARCH="arm64"
    ENABLE_CLI="OFF"
    ENABLE_TESTS="OFF"
fi

cmake \
    -G Xcode \
    -D CMAKE_OSX_ARCHITECTURES="${OSX_ARCH}" \
    -D CMAKE_BUILD_TYPE="${BUILD_TYPE}" \
    -D ENABLE_OPENMP="ON" \
    -D ENABLE_CLI="${ENABLE_CLI}" \
    -D ENABLE_TESTS="${ENABLE_TESTS}" \
    -D ENABLE_SANITIZERS="${ENABLE_SANITIZERS}" \
    -D ENABLE_LTO="${ENABLE_LTO}" \
    ..

# cmake --build . --config "${BUILD_TYPE}" -j $(sysctl -n hw.ncpu)
xcodebuild -target ALL_BUILD -project SolveSpace.xcodeproj -arch ${OSX_ARCH}
#  test_solvespace