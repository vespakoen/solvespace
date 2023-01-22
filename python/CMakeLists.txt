# cmake_minimum_required(VERSION 3.15...3.25)

# project(${SKBUILD_PROJECT_NAME} LANGUAGES CXX VERSION ${SKBUILD_PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/pybind11)

# find_package(Eigen3 REQUIRED)
# if(Eigen3_FOUND)
#   include_directories(${EIGEN_INCLUDE_DIR})
# endif()

# find_package(mimalloc CONFIG REQUIRED)
# if(mimalloc_FOUND)
#   include_directories(${MIMALLOC_INCLUDE_DIR})
# endif()

# get_cmake_property(_variableNames VARIABLES)
# list (SORT _variableNames)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()

pybind11_add_module(
    slvs
MODULE
    ${CMAKE_SOURCE_DIR}/src/core/vector.cpp
    ${CMAKE_SOURCE_DIR}/src/core/vector4.cpp
    ${CMAKE_SOURCE_DIR}/src/core/constraintbase.cpp
    ${CMAKE_SOURCE_DIR}/src/core/entitybase.cpp
    ${CMAKE_SOURCE_DIR}/src/core/expr.cpp
    ${CMAKE_SOURCE_DIR}/src/core/group.cpp
    ${CMAKE_SOURCE_DIR}/src/core/lib.cpp
    ${CMAKE_SOURCE_DIR}/src/core/minimalplatform.cpp
    ${CMAKE_SOURCE_DIR}/src/core/point2d.cpp
    ${CMAKE_SOURCE_DIR}/src/core/quaternion.cpp
    ${CMAKE_SOURCE_DIR}/src/core/sketch.cpp
    ${CMAKE_SOURCE_DIR}/src/core/solversystem.cpp
    ${CMAKE_SOURCE_DIR}/src/core/system.cpp
    ${CMAKE_SOURCE_DIR}/src/core/utils.cpp
    ${CMAKE_SOURCE_DIR}/src/core/main.cpp
)

target_include_directories(slvs PRIVATE ${EIGEN3_INCLUDE_DIRS})

target_compile_definitions(slvs PRIVATE VERSION_INFO=${PROJECT_VERSION})

target_link_libraries(slvs PRIVATE mimalloc-static)

install(TARGETS slvs DESTINATION ${SKBUILD_PROJECT_NAME})
