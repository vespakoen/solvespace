include_directories(
    ${CMAKE_SOURCE_DIR}/test
    ${CMAKE_SOURCE_DIR}/src
)

include_directories(${CAIRO_INCLUDE_DIRS})
link_directories(${CAIRO_LIBRARY_DIRS})

if(WIN32)
    add_definitions(-DTEST_BUILD_ON_WINDOWS)
endif()

# test suite
set(testsuite_SOURCES
    ${CMAKE_SOURCE_DIR}/test/harness.cpp
    ${CMAKE_SOURCE_DIR}/test/analysis/contour_area/test.cpp
    ${CMAKE_SOURCE_DIR}/test/core/expr/test.cpp
    ${CMAKE_SOURCE_DIR}/test/core/locale/test.cpp
    ${CMAKE_SOURCE_DIR}/test/core/path/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/points_coincident/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/pt_pt_distance/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/pt_plane_distance/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/pt_line_distance/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/pt_face_distance/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/proj_pt_distance/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/pt_in_plane/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/pt_on_line/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/pt_on_face/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/equal_length_lines/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/length_ratio/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/eq_len_pt_line_d/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/eq_pt_ln_distances/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/equal_angle/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/equal_line_arc_len/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/length_difference/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/symmetric/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/symmetric_horiz/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/symmetric_vert/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/symmetric_line/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/at_midpoint/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/horizontal/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/vertical/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/diameter/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/pt_on_circle/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/same_orientation/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/angle/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/parallel/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/perpendicular/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/arc_line_tangent/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/cubic_line_tangent/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/curve_curve_tangent/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/equal_radius/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/where_dragged/test.cpp
    ${CMAKE_SOURCE_DIR}/test/constraint/comment/test.cpp
    ${CMAKE_SOURCE_DIR}/test/request/arc_of_circle/test.cpp
    ${CMAKE_SOURCE_DIR}/test/request/circle/test.cpp
    ${CMAKE_SOURCE_DIR}/test/request/cubic/test.cpp
    ${CMAKE_SOURCE_DIR}/test/request/cubic_periodic/test.cpp
    ${CMAKE_SOURCE_DIR}/test/request/datum_point/test.cpp
    ${CMAKE_SOURCE_DIR}/test/request/image/test.cpp
    ${CMAKE_SOURCE_DIR}/test/request/line_segment/test.cpp
    ${CMAKE_SOURCE_DIR}/test/request/ttf_text/test.cpp
    ${CMAKE_SOURCE_DIR}/test/request/workplane/test.cpp
    ${CMAKE_SOURCE_DIR}/test/group/link/test.cpp
    ${CMAKE_SOURCE_DIR}/test/group/translate_asy/test.cpp
    ${CMAKE_SOURCE_DIR}/test/group/translate_nd/test.cpp
)

add_executable(solvespace-testsuite
    ${testsuite_SOURCES}
    $<TARGET_PROPERTY:resources,EXTRA_SOURCES>)

target_link_libraries(solvespace-testsuite
    solvespace-headless
    ${COVERAGE_LIBRARY})

target_include_directories(solvespace-testsuite
    PRIVATE
    ${EIGEN3_INCLUDE_DIRS})

add_dependencies(solvespace-testsuite
    resources)

add_custom_target(test_solvespace
    COMMAND $<TARGET_FILE:solvespace-testsuite>
    COMMENT "Testing SolveSpace"
    VERBATIM)

# coverage reports

if(ENABLE_COVERAGE)
    set(LCOV_FLAGS   -q --gcov-tool ${GCOV})
    set(LCOV_FLAGS   ${LCOV_FLAGS} --rc lcov_branch_coverage=1)
    set(LCOV_FLAGS   ${LCOV_FLAGS} --rc "lcov_excl_line=(ssassert|switch)")
    set(LCOV_FLAGS   ${LCOV_FLAGS} --rc "lcov_excl_br_line=BRANCH_ALWAYS_TAKEN")
    set(LCOV_COLLECT -c -b ${CMAKE_SOURCE_DIR}/src -d ${CMAKE_BINARY_DIR}/src --no-external)

    add_custom_command(
        OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/coverage_base.info
        COMMAND ${LCOV} ${LCOV_FLAGS} ${LCOV_COLLECT}
            -o ${CMAKE_BINARY_DIR}/coverage_base.info -i
        DEPENDS solvespace-testsuite
        COMMENT "Importing baseline coverage data"
        VERBATIM)

    add_custom_target(coverage_solvespace ALL
        COMMAND ${LCOV} ${LCOV_FLAGS} ${LCOV_COLLECT}
            -o ${CMAKE_BINARY_DIR}/coverage_test.info
        COMMAND ${LCOV} ${LCOV_FLAGS}
            -o ${CMAKE_BINARY_DIR}/coverage_full.info
            -a ${CMAKE_BINARY_DIR}/coverage_base.info
            -a ${CMAKE_BINARY_DIR}/coverage_test.info
        COMMAND ${LCOV} ${LCOV_FLAGS} --summary
            ${CMAKE_BINARY_DIR}/coverage_full.info
        COMMAND ${GENHTML} -q --branch-coverage --demangle-cpp --legend
            ${CMAKE_BINARY_DIR}/coverage_full.info
            -o ${CMAKE_BINARY_DIR}/coverage/
            -t "SolveSpace testbench"
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/coverage_base.info
        DEPENDS test_solvespace
        COMMENT "Generating coverage report"
        VERBATIM)
endif()

# debug runner

set(debugtool_SOURCES
    debugtool.cpp
)

add_executable(solvespace-debugtool
    ${debugtool_SOURCES}
    $<TARGET_PROPERTY:resources,EXTRA_SOURCES>)

target_link_libraries(solvespace-debugtool
    solvespace-core
    solvespace-headless)

add_dependencies(solvespace-debugtool
    resources)

if(ENABLE_OPENMP AND APPLE)
    # add homebrew libomp paths to the INSTALL_RPATH, and the @loader_path last as a fallback.
    set_target_properties(solvespace-testsuite PROPERTIES
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH "/opt/homebrew/opt/libomp/lib;/usr/local/opt/libomp/lib;@loader_path")
    set_target_properties(solvespace-debugtool PROPERTIES
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH "/opt/homebrew/opt/libomp/lib;/usr/local/opt/libomp/lib;@loader_path")
    # copy libomp to and fix loader paths
    add_custom_command(TARGET solvespace-testsuite POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${OpenMP_CXX_LIBRARIES} $<TARGET_FILE_DIR:solvespace-testsuite>/libomp.dylib
        COMMAND ${CMAKE_INSTALL_NAME_TOOL} -change `otool -L $<TARGET_FILE:solvespace-testsuite> | grep libomp | cut -d ' ' -f1 | xargs echo` "@rpath/libomp.dylib" $<TARGET_FILE:solvespace-testsuite>)
    add_custom_command(TARGET solvespace-debugtool POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${OpenMP_CXX_LIBRARIES} $<TARGET_FILE_DIR:solvespace-debugtool>/libomp.dylib
        COMMAND ${CMAKE_INSTALL_NAME_TOOL} -change `otool -L $<TARGET_FILE:solvespace-debugtool> | grep libomp | cut -d ' ' -f1 | xargs echo` "@rpath/libomp.dylib" $<TARGET_FILE:solvespace-debugtool>)
endif()
