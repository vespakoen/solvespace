set(libslvs_SOURCES
    ${CMAKE_SOURCE_DIR}/src/util.cpp
    ${CMAKE_SOURCE_DIR}/src/entity.cpp
    ${CMAKE_SOURCE_DIR}/src/expr.cpp
    ${CMAKE_SOURCE_DIR}/src/constraint.cpp
    ${CMAKE_SOURCE_DIR}/src/constrainteq.cpp
    ${CMAKE_SOURCE_DIR}/src/system.cpp
    ${CMAKE_SOURCE_DIR}/src/platform/platform.cpp)

set(libslvs_HEADERS
    ${CMAKE_SOURCE_DIR}/src/solvespace.h
    ${CMAKE_SOURCE_DIR}/src/platform/platform.h)

add_library(slvs SHARED
    ${libslvs_SOURCES}
    ${libslvs_HEADERS}
    ${CMAKE_SOURCE_DIR}/src/lib.cpp)

target_compile_definitions(slvs
    PRIVATE -DLIBRARY)

target_include_directories(slvs
    PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(slvs
    PRIVATE
    ${util_LIBRARIES}
    mimalloc-static)

set_target_properties(slvs PROPERTIES
    PUBLIC_HEADER ${CMAKE_SOURCE_DIR}/include/slvs.h
    VERSION ${solvespace_VERSION_MAJOR}.${solvespace_VERSION_MINOR}
    SOVERSION 1)

if(NOT WIN32)
    install(TARGETS slvs
        LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE       DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()