# MIMALLOC
message(STATUS "Using in-tree mimalloc")
set(MI_OVERRIDE OFF CACHE BOOL "")
set(MI_BUILD_SHARED OFF CACHE BOOL "")
set(MI_BUILD_OBJECT OFF CACHE BOOL "")
set(MI_BUILD_TESTS OFF CACHE BOOL "")
add_subdirectory(extlib/mimalloc EXCLUDE_FROM_ALL)
set(MIMALLOC_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/extlib/mimalloc/include)

# EIGEN
if(NOT FORCE_VENDORED_Eigen3)
    find_package(Eigen3 CONFIG)
endif()
if(FORCE_VENDORED_Eigen3 OR NOT EIGEN3_INCLUDE_DIRS)
    message(STATUS "Using in-tree Eigen")
    set(EIGEN3_FOUND             YES)
    set(EIGEN3_INCLUDE_DIRS      ${CMAKE_SOURCE_DIR}/extlib/eigen)
else()
    message(STATUS "Using system Eigen: ${EIGEN3_INCLUDE_DIRS}")
endif()
if(NOT EXISTS "${EIGEN3_INCLUDE_DIRS}")
    message(FATAL_ERROR "Eigen 3 not found on system or in-tree")
endif()

# OPENMP
if(ENABLE_OPENMP)
    if(APPLE)
        list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/libomp")
        list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/libomp")
    endif()
    find_package(OpenMP REQUIRED)
    if(OPENMP_FOUND)
        add_library(slvs_openmp INTERFACE)
        target_link_libraries(slvs_openmp INTERFACE OpenMP::OpenMP_CXX)
        message(STATUS "found OpenMP, compiling with flags: " ${OpenMP_CXX_FLAGS} )
    endif()
endif()

if(ENABLE_GUI OR ENABLE_CLI)
    message(STATUS "Using in-tree libdxfrw")
    add_subdirectory(extlib/libdxfrw)

    if(WIN32 OR APPLE OR EMSCRIPTEN)
        # On Win32 and macOS we use vendored packages, since there is little to no benefit
        # to trying to find system versions. In particular, trying to link to libraries from
        # Homebrew or macOS system libraries into the .app file is highly likely to result
        # in incompatibilities after upgrades.

        include(FindVendoredPackage)
        include(AddVendoredSubdirectory)

        set(FORCE_VENDORED_ZLIB     ON)
        set(FORCE_VENDORED_PNG      ON)
        set(FORCE_VENDORED_Freetype ON)

        find_vendored_package(ZLIB zlib
            ZLIB_LIBRARY            zlibstatic
            ZLIB_INCLUDE_DIR        ${CMAKE_SOURCE_DIR}/extlib/zlib)
        list(APPEND ZLIB_INCLUDE_DIR ${CMAKE_BINARY_DIR}/extlib/zlib)

        find_vendored_package(PNG libpng
            SKIP_INSTALL_ALL        ON
            PNG_LIBRARY             png_static
            PNG_ARM_NEON            "off"
            PNG_SHARED              OFF
            PNG_STATIC              ON
            PNG_EXECUTABLES         OFF
            PNG_TESTS               OFF
            PNG_FRAMEWORK           OFF
            PNG_PNG_INCLUDE_DIR     ${CMAKE_SOURCE_DIR}/extlib/libpng)
        list(APPEND PNG_PNG_INCLUDE_DIR ${CMAKE_BINARY_DIR}/extlib/libpng)

        find_vendored_package(Freetype freetype
            WITH_ZLIB               OFF
            WITH_BZip2              OFF
            WITH_PNG                OFF
            WITH_HarfBuzz           OFF
            FREETYPE_LIBRARY        freetype
            FREETYPE_INCLUDE_DIRS   ${CMAKE_SOURCE_DIR}/extlib/freetype/include)

        message(STATUS "Using in-tree pixman")
        set(PIXMAN_FOUND            YES)
        set(PIXMAN_LIBRARY          pixman)
        set(PIXMAN_BUILD_TESTS      OFF CACHE BOOL "")
        set(PIXMAN_BUILD_DEMOS      OFF CACHE BOOL "")

        set(PIXMAN_INCLUDE_DIRS     ${CMAKE_SOURCE_DIR}/extlib/pixman/pixman)
        list(APPEND PIXMAN_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/extlib/pixman/pixman)
        add_vendored_subdirectory(extlib/pixman)

        message(STATUS "Using in-tree cairo")
        add_vendored_subdirectory(extlib/cairo)
        set(CAIRO_FOUND             YES)
        set(CAIRO_LIBRARIES         cairo)
        set(CAIRO_INCLUDE_DIRS      ${CMAKE_SOURCE_DIR}/extlib/cairo/src)
        list(APPEND CAIRO_INCLUDE_DIRS ${CMAKE_BINARY_DIR}/extlib/cairo/src)
    else()
        # On Linux and BSDs we're a good citizen and link to system libraries.
        find_package(Backtrace)
        find_package(PkgConfig REQUIRED)
        find_package(ZLIB REQUIRED)
        find_package(PNG REQUIRED)
        find_package(Freetype REQUIRED)
        find_package(Cairo REQUIRED)
    endif()

    find_program(XGETTEXT xgettext)
    find_program(MSGINIT  msginit)
    find_program(MSGMERGE msgmerge)
    if(XGETTEXT AND MSGINIT AND MSGMERGE)
        set(HAVE_GETTEXT TRUE)
    else()
        message(WARNING "Gettext not found, translations will not be updated")
        set(HAVE_GETTEXT FALSE)
    endif()

    if(APPLE)
        find_library(APPKIT_LIBRARY AppKit REQUIRED)
    endif()

    # GUI dependencies
    if(ENABLE_GUI)
        if(WIN32)
            if(OPENGL STREQUAL "3")
                message(STATUS "Using in-tree ANGLE")
                set(ANGLE_STATIC            ON  CACHE INTERNAL "")
                set(ANGLE_ENABLE_D3D9       ON  CACHE INTERNAL "")
                set(ANGLE_ENABLE_D3D11      ON  CACHE INTERNAL "")
                set(ANGLE_ENABLE_OPENGL     ON  CACHE INTERNAL "")
                set(ANGLE_ENABLE_ESSL       ON  CACHE INTERNAL "")
                set(ANGLE_ENABLE_GLSL       ON  CACHE INTERNAL "")
                set(ANGLE_ENABLE_HLSL       ON  CACHE INTERNAL "")
                add_vendored_subdirectory(extlib/angle)
                set(OPENGL_LIBRARIES        EGL GLESv2)
                set(OPENGL_INCLUDE_DIR      ${CMAKE_SOURCE_DIR}/extlib/angle/include)
            else()
                find_package(OpenGL REQUIRED)
            endif()

            if(MSVC AND ${CMAKE_SIZEOF_VOID_P} EQUAL 4)
                message(STATUS "Using prebuilt SpaceWare")
                set(SPACEWARE_FOUND TRUE)
                set(SPACEWARE_INCLUDE_DIR
                    "${CMAKE_SOURCE_DIR}/extlib/si")
                set(SPACEWARE_LIBRARIES
                    "${CMAKE_SOURCE_DIR}/extlib/si/siapp.lib")
            endif()
        elseif(APPLE)
            find_package(OpenGL REQUIRED)
        elseif(EMSCRIPTEN)
            # Everything is built in
        else()
            find_package(OpenGL REQUIRED)
            find_package(SpaceWare)
            pkg_check_modules(FONTCONFIG REQUIRED fontconfig)
            pkg_check_modules(JSONC REQUIRED json-c)
            pkg_check_modules(GTKMM REQUIRED gtkmm-3.0>=3.18 pangomm-1.4 x11)
        endif()
    endif()
endif()