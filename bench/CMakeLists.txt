# benchmark runner

include_directories(${CAIRO_INCLUDE_DIRS})
link_directories(${CAIRO_LIBRARY_DIRS})

add_executable(solvespace-benchmark
    harness.cpp
    $<TARGET_PROPERTY:resources,EXTRA_SOURCES>)

target_link_libraries(solvespace-benchmark
    solvespace-core
    solvespace-headless)

add_dependencies(solvespace-benchmark
    resources)

if(ENABLE_OPENMP AND APPLE)
    # add homebrew libomp paths to the INSTALL_RPATH, and the @loader_path last as a fallback.
    set_target_properties(solvespace-benchmark PROPERTIES
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH "/opt/homebrew/opt/libomp/lib;/usr/local/opt/libomp/lib;@loader_path")
    # fix loader path
    add_custom_command(TARGET solvespace-benchmark POST_BUILD
        COMMAND ${CMAKE_INSTALL_NAME_TOOL} -change `otool -L $<TARGET_FILE:solvespace-benchmark> | grep libomp | cut -d ' ' -f1 | xargs echo` "@rpath/libomp.dylib" $<TARGET_FILE:solvespace-benchmark>)
    # copy libomp to solvespace-benchmark's folder
    add_custom_command(TARGET solvespace-benchmark POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${OpenMP_CXX_LIBRARIES} $<TARGET_FILE_DIR:solvespace-benchmark>/libomp.dylib)
endif()
