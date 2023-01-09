message(STATUS "Building CLI")

# solvespace headless library

add_library(
    solvespace-headless
STATIC EXCLUDE_FROM_ALL
    ${CMAKE_SOURCE_DIR}/src/solvespace.cpp
    ${CMAKE_SOURCE_DIR}/src/platform/guinone.cpp
    ${CMAKE_SOURCE_DIR}/src/render/rendercairo.cpp
)

target_compile_definitions(solvespace-headless
    PRIVATE HEADLESS)

# target_include_directories(
#     solvespace-headless
# PUBLIC
#     ${CMAKE_SOURCE_DIR}/include
#     ${EIGEN3_INCLUDE_DIRS}
# )

target_link_libraries(
    solvespace-headless
PRIVATE
    solvespace-core
)

# solvespace command-line executable

add_executable(solvespace-cli
    ${CMAKE_SOURCE_DIR}/src/platform/entrycli.cpp
    $<TARGET_PROPERTY:resources,EXTRA_SOURCES>)

target_link_libraries(
    solvespace-cli
PRIVATE
    solvespace-core
    solvespace-headless)

add_dependencies(solvespace-cli
    resources)

if(MSVC)
    set_target_properties(solvespace-cli PROPERTIES
        LINK_FLAGS "/INCREMENTAL:NO /OPT:REF")
endif()

if(NOT (WIN32 OR APPLE OR EMSCRIPTEN))
    install(TARGETS solvespace-cli
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

if(ENABLE_OPENMP AND APPLE)
    # add homebrew libomp paths to the INSTALL_RPATH, and the @loader_path last as a fallback.
    set_target_properties(solvespace-cli PROPERTIES
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH "/opt/homebrew/opt/libomp/lib;/usr/local/opt/libomp/lib;@loader_path/../Resources;@loader_path")
    # fix loader path
    add_custom_command(TARGET solvespace-cli POST_BUILD
        COMMAND ${CMAKE_INSTALL_NAME_TOOL} -change `otool -L $<TARGET_FILE:solvespace-cli> | grep libomp | cut -d ' ' -f1 | xargs echo` "@rpath/libomp.dylib" $<TARGET_FILE:solvespace-cli>
    )
    # copy libomp into install directory
    if(ENABLE_GUI)
        # The GUI is enabled, copy the cli into the correct folders
        add_custom_command(TARGET solvespace-gui POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:solvespace-cli> $<TARGET_FILE_DIR:solvespace-gui>
            COMMENT "Bundling executable solvespace-cli"
            VERBATIM)
    else()
        # GUI is not enabled, copy libomp to solvespace-cli's folder
        add_custom_command(TARGET solvespace-cli POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${OpenMP_CXX_LIBRARIES} $<TARGET_FILE_DIR:solvespace-cli>/libomp.dylib)
    endif()
endif()
