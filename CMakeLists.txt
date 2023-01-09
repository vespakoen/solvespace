cmake_minimum_required(VERSION 3.9...3.19)

#=========#
# OPTIONS #
#=========#

option(ENABLE_LIB
    "Whether the c library is enabled" OFF)
option(ENABLE_GUI
    "Whether the graphical interface is enabled" ON)
option(ENABLE_CLI
    "Whether the command line interface is enabled" ON)
option(ENABLE_TESTS
    "Whether the test suite will be built and run" ON)
option(ENABLE_COVERAGE
    "Whether code coverage information will be collected" OFF)
option(ENABLE_SANITIZERS
    "Whether to enable Clang's AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
option(ENABLE_OPENMP
    "Whether geometric operations will be parallelized using OpenMP" OFF)
option(ENABLE_LTO
    "Whether interprocedural (global) optimizations are enabled" OFF)
option(ENABLE_BENCH
    "Whether benchmarks are enabled" OFF)
option(FORCE_VENDORED_Eigen3
    "Whether we should use our bundled Eigen even in the presence of a system copy" OFF)
set(OPENGL 3 CACHE STRING "OpenGL version to use (one of: 1 3)")

#======#
# MISC #
#======#

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR
            "In-tree builds are not supported; please perform an out-of-tree build:\n"
            "    rm -rf CMakeCache.txt CMakeFiles/\n"
            "    mkdir build && cd build && cmake ..")
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH    ${CMAKE_BINARY_DIR}/bin)
if("${CMAKE_GENERATOR}" STREQUAL "Xcode")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_BINARY_DIR}/bin>)
endif()

# for /MT on MSVC
set(CMAKE_USER_MAKE_RULES_OVERRIDE
    "${CMAKE_SOURCE_DIR}/cmake/c_flag_overrides.cmake")
set(CMAKE_USER_MAKE_RULES_OVERRIDE_CXX
    "${CMAKE_SOURCE_DIR}/cmake/cxx_flag_overrides.cmake")

if(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

if (APPLE)
    # Docs say this must be set before the first project() call
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12" CACHE STRING "macOS minimum supported version")
endif()

list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_SOURCE_DIR}/cmake/")

# NOTE TO PACKAGERS: The embedded git commit hash is critical for rapid bug triage when the builds
# can come from a variety of sources. If you are mirroring the sources or otherwise build when
# the .git directory is not present, please comment the following line:
include(GetGitCommitHash)
# and instead uncomment the following, adding the complete git hash of the checkout you are using:
# set(GIT_COMMIT_HASH 0000000000000000000000000000000000000000)

string(SUBSTRING "${GIT_COMMIT_HASH}" 0 8 solvespace_GIT_HASH)

project(SolveSpace
    VERSION 3.1
    LANGUAGES C CXX ASM)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0)
        # GCC 4.8/4.9 ship with broken but present <regex>. meh.
        message(FATAL_ERROR "GCC 5.0+ is required")
    endif()
endif()

# common compiler flags
include(CheckCXXCompilerFlag)

if (NOT APPLE)
    set(FILE_PREFIX_MAP "-ffile-prefix-map=${CMAKE_CURRENT_SOURCE_DIR}=.")
    check_cxx_compiler_flag("${FILE_PREFIX_MAP}" HAS_FILE_PREFIX_MAP)
    if(HAS_FILE_PREFIX_MAP)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FILE_PREFIX_MAP}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FILE_PREFIX_MAP}")
    endif()
endif()

if(MINGW)
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -static-libgcc")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
    # Link 32 bit SolveSpace with --large-address-aware which allows it to access
    # up to 3GB on a properly configured 32 bit Windows and up to 4GB on 64 bit.
    # See https://msdn.microsoft.com/en-us/library/aa366778
    set(CMAKE_EXE_LINKER_FLAGS "-Wl,--large-address-aware")
endif()

# Ensure that all platforms use 64-bit IEEE floating point operations for consistency;
# this is most important for the testsuite, which compares savefiles directly
# and depends on consistent rounding of intermediate results.
if(CMAKE_SYSTEM_PROCESSOR STREQUAL "i686" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "X86")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set(FLOAT_FLAGS "-mfpmath=sse -msse2")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(FLOAT_FLAGS "-msse2")
    endif()

    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} ${FLOAT_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLOAT_FLAGS}")
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    set(CMAKE_EXE_LINKER_FLAGS "-Wl,--as-needed ${CMAKE_EXE_LINKER_FLAGS}")
endif()

if(ENABLE_SANITIZERS)
    if(NOT SANITIZERS)
        set(SANITIZERS "address;undefined")
    endif()

    if("thread" IN_LIST SANITIZERS)
        list(REMOVE_ITEM SANITIZERS "thread")
        list(APPEND SANITIZE_OPTIONS thread)
    endif()
    if("address" IN_LIST SANITIZERS)
        list(REMOVE_ITEM SANITIZERS "address")
        list(APPEND SANITIZE_OPTIONS address)
    endif()
    if("undefined" IN_LIST SANITIZERS)
        list(REMOVE_ITEM SANITIZERS "undefined")
        list(APPEND SANITIZE_OPTIONS alignment bounds)
        list(APPEND SANITIZE_OPTIONS shift signed-integer-overflow integer-divide-by-zero)
        list(APPEND SANITIZE_OPTIONS null bool enum)
        list(APPEND SANITIZE_OPTIONS return)
    endif()
    if(SANITIZERS)
        message(FATAL_ERROR "Unknown sanitizer(s) ${SANITIZERS}")
    else()
        message(STATUS "Using sanitizer options ${SANITIZE_OPTIONS}")
    endif()

    string(REPLACE ";" "," SANITIZE_OPTIONS "${SANITIZE_OPTIONS}")

    if (NOT APPLE)
        set(SANITIZE_FLAGS "-O1 -fsanitize=${SANITIZE_OPTIONS} -fno-sanitize-recover=address,undefined")
    else()
        set(SANITIZE_FLAGS "-O1 -fsanitize=${SANITIZE_OPTIONS}")
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(SANITIZE_FLAGS "${SANITIZE_FLAGS} -fno-omit-frame-pointer -fno-optimize-sibling-calls")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set(SANITIZE_FLAGS "${SANITIZE_FLAGS} -fuse-ld=gold")
    else()
        message(FATAL_ERROR "Sanitizers are only available when using GCC or Clang")
    endif()

    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} ${SANITIZE_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SANITIZE_FLAGS}")
endif()

if(APPLE)
    set(CMAKE_FIND_FRAMEWORK LAST)
endif()

if(EMSCRIPTEN)
    set(M_LIBRARY "" CACHE STRING "libm (not necessary)" FORCE)
endif()

#==============#
# DEPENDENCIES #
#==============#

include(src/extlib.cmake)

#========#
# CONFIG #
#========#

set(HAVE_SPACEWARE ${SPACEWARE_FOUND})

if(NOT WIN32 OR APPLE)
    if(GTKMM_gtkmm-3.0_VERSION VERSION_LESS "3.24.0")
        set(HAVE_GTK_FILECHOOSERNATIVE 0)
    else()
        set(HAVE_GTK_FILECHOOSERNATIVE 1)
    endif()
endif()

configure_file(src/config.h.in src/config.h)

#==========#
# COVERAGE #
#==========#

if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID STREQUAL GNU)
        find_program(GCOV gcov)
    elseif(CMAKE_CXX_COMPILER_ID MATCHES Clang)
        find_program(LLVM_COV llvm-cov)

        if(LLVM_COV)
            set(GCOV ${CMAKE_CURRENT_BINARY_DIR}/llvm-gcov.sh)
            file(WRITE ${GCOV} "#!/bin/sh -e\n${LLVM_COV} gcov $*")
            execute_process(COMMAND chmod +x ${GCOV})
        endif()
    endif()

    find_program(LCOV lcov)
    find_program(GENHTML genhtml)
    if(NOT GCOV OR NOT LCOV OR NOT GENHTML)
        message(FATAL_ERROR "gcov/llvm-cov and lcov are required for producing coverage reports")
    endif()
endif()

# solvespace-only compiler flags
if(WIN32)
    add_definitions(
        -D_CRT_SECURE_NO_DEPRECATE
        -D_CRT_SECURE_NO_WARNINGS
        -D_SCL_SECURE_NO_WARNINGS
        -DWINVER=0x0501
        -D_WIN32_WINNT=0x0501
        -D_WIN32_IE=_WIN32_WINNT
        -DISOLATION_AWARE_ENABLED
        -DWIN32
        -DWIN32_LEAN_AND_MEAN
        -DUNICODE
        -D_UNICODE
        -DNOMINMAX
        -D_USE_MATH_DEFINES)
endif()

if(MSVC)
    # Many versions of MSVC do not have the (C99) inline keyword, instead
    # they have their own __inline; this breaks `static inline` functions.
    # We do not want to care and so we fix this with a definition.
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Dinline=__inline")
    # Same for the (C99) __func__ special variable; we use it only in C++ code.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D__func__=__FUNCTION__")

    # Multi-processor Compilation
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

    # We rely on these /we flags. They correspond to the GNU-style flags below as
    # follows: /w4062=-Wswitch
    set(WARNING_FLAGS   "${WARNING_FLAGS} /we4062")

    # Link 32 bit SolveSpace with /LARGEADDRESSAWARE which allows it to access
    # up to 3GB on a properly configured 32 bit Windows and up to 4GB on 64 bit.
    # See https://msdn.microsoft.com/en-us/library/aa366778
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LARGEADDRESSAWARE")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /LARGEADDRESSAWARE")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILE_ID MATCHES "AppleClang")
    set(WARNING_FLAGS   "-Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -Wunused-command-line-argument")
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
        set(WARNING_FLAGS "${WARNING_FLAGS} -Wfloat-conversion")
    endif()
    # We rely on these -Werror flags.
    set(WARNING_FLAGS   "${WARNING_FLAGS} -Werror=switch")
endif()

set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} ${WARNING_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS}")

if(WIN32)
    set(CMAKE_RC_FLAGS  "${CMAKE_RC_FLAGS} -l0")
endif()

if(ENABLE_COVERAGE)
    if(NOT (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR
            CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
        message(FATAL_ERROR "Code coverage is only available on GCC and Clang")
    endif()

    if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(FATAL_ERROR "Code coverage produces reliable results only on Debug builds")
    endif()

    # With -fexceptions, every call becomes a branch. While technically accurate,
    # this is not useful for us.
    set(COVERAGE_FLAGS -fno-exceptions --coverage)
    set(COVERAGE_LIBRARY --coverage)
endif()

if(ENABLE_LIB)
    include(src/libslvs.cmake)
endif()

if(ENABLE_GUI OR ENABLE_CLI)
    include(src/core.cmake)
    add_subdirectory(res)
endif()

if(ENABLE_GUI)
    include(src/gui.cmake)
endif()

if(ENABLE_CLI)
    include(src/cli.cmake)
endif()

if(ENABLE_TESTS)
    add_subdirectory(test)
endif()

if(ENABLE_BENCH)
    if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        add_subdirectory(bench)
    else()
        message(STATUS "Benchmarking disabled in debug builds.")
  endif()
endif()

if(APPLE)
  set(CPACK_DMG_VOLUME_NAME "SolveSpace")
  include(CPack)
endif()