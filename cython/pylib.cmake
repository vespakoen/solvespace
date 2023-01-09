find_package(Python3 COMPONENTS Interpreter Development.Module REQUIRED)
if(Python3_FOUND)
    message(STATUS "Found Python: " ${Python3_VERSION})
    message(STATUS "Python libraries: " ${Python3_LIBRARIES})
    message(STATUS "Python executable: " ${Python3_EXECUTABLE})
endif()

# this makes the slvs Python module
Python3_add_library(
    slvs
MODULE
    ${CMAKE_SOURCE_DIR}/cython/solvespace/slvs.cpp
    ${CMAKE_SOURCE_DIR}/cython/solvespace/slvs.pyx
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

target_link_libraries(
    slvs
PRIVATE
    mimalloc-static
    Python3::Module
)

if(USE_OPENMP)
    target_link_libraries(slvs PRIVATE OpenMP::OpenMP_CXX)
endif()

install(TARGETS slvs LIBRARY DESTINATION "solvespace")
if(NOT SKBUILD)
  install(
    DIRECTORY cython/solvespace/
    DESTINATION "solvespace"
  )
endif()

if(USE_OPENMP AND APPLE)
  # copy libomp into install directory
  install(
    FILES ${OpenMP_CXX_LIBRARIES}
    DESTINATION "solvespace"
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
  )
  # fix loader path
  add_custom_command(TARGET slvs POST_BUILD
    COMMAND ${CMAKE_INSTALL_NAME_TOOL} -change `otool -L $<TARGET_FILE:slvs> | grep libomp | cut -d ' ' -f1 | xargs echo` "@loader_path/libomp.dylib" $<TARGET_FILE:slvs>
  )
endif()

# add include directories
target_include_directories(
    slvs
PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
    ${MIMALLOC_INCLUDE_DIR}
    ${EIGEN3_INCLUDE_DIRS}
)

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