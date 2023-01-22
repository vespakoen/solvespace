cmake_minimum_required(VERSION 3.15...3.25)

project(${SKBUILD_PROJECT_NAME} LANGUAGES CXX VERSION ${SKBUILD_PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(${CMAKE_SOURCE_DIR}/pybind11)

find_package(Eigen3 REQUIRED)
if(Eigen3_FOUND)
  include_directories(${EIGEN_INCLUDE_DIR})
endif()

find_package(mimalloc CONFIG REQUIRED)
if(mimalloc_FOUND)
  include_directories(${MIMALLOC_INCLUDE_DIR})
endif()

# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()

pybind11_add_module(
    slvs
MODULE
    src/vector.cpp
    src/vector4.cpp
    src/constraintbase.cpp
    src/entitybase.cpp
    src/expr.cpp
    src/group.cpp
    src/lib.cpp
    src/minimalplatform.cpp
    src/point2d.cpp
    src/quaternion.cpp
    src/sketch.cpp
    src/solversystem.cpp
    src/system.cpp
    src/utils.cpp
    src/main.cpp
)

target_compile_definitions(slvs PRIVATE VERSION_INFO=${PROJECT_VERSION})

target_link_libraries(slvs PRIVATE mimalloc Eigen3::Eigen)

install(TARGETS slvs DESTINATION ${SKBUILD_PROJECT_NAME})
