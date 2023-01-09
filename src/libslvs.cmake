message(STATUS "Building libslvs")

add_library(
    slvs
SHARED
    ${CMAKE_SOURCE_DIR}/src/solvespace.h
    ${CMAKE_SOURCE_DIR}/src/platform/minimalplatform.h
    ${CMAKE_SOURCE_DIR}/src/util.cpp
    ${CMAKE_SOURCE_DIR}/src/entity.cpp
    ${CMAKE_SOURCE_DIR}/src/expr.cpp
    ${CMAKE_SOURCE_DIR}/src/constraint.cpp
    ${CMAKE_SOURCE_DIR}/src/constrainteq.cpp
    ${CMAKE_SOURCE_DIR}/src/system.cpp
    ${CMAKE_SOURCE_DIR}/src/platform/minimalplatform.cpp
    ${CMAKE_SOURCE_DIR}/src/lib.cpp
)

#============#
# PROPERTIES #
#============#

if(WIN32)
  # on windows, prefix the library with lib, and make sure the .lib file is installed as well
  set_target_properties(slvs PROPERTIES
      PREFIX "lib"
      WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

set_target_properties(
  slvs
PROPERTIES
    PUBLIC_HEADER ${CMAKE_SOURCE_DIR}/include/slvs.h
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

#=====================#
# INCLUDE DIRECTORIES #
#=====================#

# add include directories
target_include_directories(
    slvs
PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
    ${MIMALLOC_INCLUDE_DIR}
    ${EIGEN3_INCLUDE_DIRS}
  PUBLIC
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

#======#
# LINK #
#======#

target_link_libraries(slvs PRIVATE mimalloc-static)
if(ENABLE_OPENMP)
  target_link_libraries(slvs PRIVATE OpenMP::OpenMP_CXX)
endif()

#===================#
# MACRO DEFINITIONS #
#===================#

target_compile_definitions(slvs PRIVATE M_PI=PI _USE_MATH_DEFINES ISOLATION_AWARE_ENABLED LIBRARY _CRT_SECURE_NO_WARNINGS)

#=====#
# LTO #
#=====#

if(ENABLE_LTO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT have_ipo_support)
    if(have_ipo_support)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()
endif()

#=========#
# INSTALL #
#=========#

include(GNUInstallDirs)
install(
    TARGETS slvs
    EXPORT slvstargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/solvespace
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/solvespace
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/solvespace
)

if(APPLE)
  target_compile_options(slvs PRIVATE
    $<$<CXX_COMPILER_ID:Clang>:-g>
  )
  set(strip_command COMMAND xcrun strip -Sl $<TARGET_FILE:slvs>)
  if(CMAKE_INTERPROCEDURAL_OPTIMIZATION)
    set(lto_object ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/slvs-lto.o)
    set_property(TARGET slvs APPEND_STRING PROPERTY
      LINK_FLAGS " -Wl,-object_path_lto -Wl,${lto_object}")
  endif()
  add_custom_command(TARGET slvs POST_BUILD
    COMMAND xcrun dsymutil $<TARGET_FILE:slvs>
    ${strip_command})
  install(
    FILES $<TARGET_FILE:slvs>.dSYM
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/solvespace
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
  )
endif()

# this install the cmake targets
install(
  EXPORT slvstargets
  FILE SolveSpaceTargets.cmake
  DESTINATION share/cmake/solvespace
  NAMESPACE SolveSpace::
)

# this creates the cmake config
include(CMakePackageConfigHelpers)
configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake
)

# this installs the cmake config
install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/${PROJECT_NAME}
)

if(ENABLE_OPENMP)
  if(APPLE)
    # copy libomp into install directory
    install(
      FILES ${OpenMP_CXX_LIBRARIES}
      DESTINATION ${CMAKE_INSTALL_LIBDIR}/solvespace
      PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
    )
    # fix loader path
    add_custom_command(TARGET slvs POST_BUILD
      COMMAND ${CMAKE_INSTALL_NAME_TOOL} -change `otool -L $<TARGET_FILE:slvs> | grep libomp | cut -d ' ' -f1 | xargs echo` "@loader_path/libomp.dylib" $<TARGET_FILE:slvs>
    )
  endif()
endif()
