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
    core/vector.cpp
    core/vector4.cpp
    core/constraintbase.cpp
    core/entitybase.cpp
    core/expr.cpp
    core/group.cpp
    core/lib.cpp
    core/minimalplatform.cpp
    core/point2d.cpp
    core/quaternion.cpp
    core/sketch.cpp
    core/solversystem.cpp
    core/system.cpp
    core/utils.cpp
    core/main.cpp
)

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_DIR}/../src/core ${CMAKE_CURRENT_BINARY_DIR}/src
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_DIR}/../extlib/eigen ${CMAKE_CURRENT_BINARY_DIR}/extlib/eigen
)
add_dependencies(slvs copy_assets)

target_compile_definitions(slvs PRIVATE VERSION_INFO=${PROJECT_VERSION})

target_link_libraries(slvs PRIVATE mimalloc Eigen3::Eigen)

install(TARGETS slvs DESTINATION ${SKBUILD_PROJECT_NAME})
