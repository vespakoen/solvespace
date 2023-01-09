message(STATUS "Building GUI")

#================#
# SOLVESPACE GUI #
#================#

target_compile_options(solvespace-core PRIVATE -g)

add_executable(solvespace-gui WIN32 MACOSX_BUNDLE
    ${CMAKE_SOURCE_DIR}/src/solvespace.cpp
    ${CMAKE_SOURCE_DIR}/src/platform/entrygui.cpp
    $<TARGET_PROPERTY:resources,EXTRA_SOURCES>)

target_include_directories(
    solvespace-gui
PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/src
    ${MIMALLOC_INCLUDE_DIR}
    ${EIGEN3_INCLUDE_DIRS}
)

add_dependencies(solvespace-gui resources)

target_link_libraries(
    solvespace-gui
PRIVATE
    solvespace-core
    ${OPENGL_LIBRARIES}
)

# OpenGL version
if(OPENGL STREQUAL 3)
    target_sources(solvespace-gui PRIVATE
        ${CMAKE_SOURCE_DIR}/src/render/gl3shader.cpp
        ${CMAKE_SOURCE_DIR}/src/render/rendergl3.cpp)
elseif(OPENGL STREQUAL 1)
    target_sources(solvespace-gui PRIVATE
        ${CMAKE_SOURCE_DIR}/src/render/rendergl1.cpp)
else()
    message(FATAL_ERROR "Unsupported OpenGL version ${OPENGL}")
endif()

# Platform-specific
if(WIN32)
    target_sources(solvespace-gui PRIVATE
        ${CMAKE_SOURCE_DIR}/src/platform/guiwin.cpp)

    target_link_libraries(solvespace-gui PRIVATE comctl32)
elseif(APPLE)
    target_compile_options(solvespace-gui PRIVATE -fobjc-arc)

    target_compile_definitions(solvespace-gui PRIVATE GL_SILENCE_DEPRECATION)

    target_sources(solvespace-gui PRIVATE
        ${CMAKE_SOURCE_DIR}/src/platform/guimac.mm)

    set_target_properties(solvespace-gui PROPERTIES
        OUTPUT_NAME SolveSpace
        XCODE_ATTRIBUTE_ENABLE_HARDENED_RUNTIME "YES"
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.solvespace"
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH "/opt/homebrew/opt/libomp/lib;/usr/local/opt/libomp/lib;@loader_path/../Resources;@loader_path"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
elseif(EMSCRIPTEN)
    set(SHELL ${CMAKE_CURRENT_SOURCE_DIR}/platform/html/emshell.html)
    set(LINK_FLAGS
        --bind --shell-file ${SHELL}
        --no-heap-copy -s ALLOW_MEMORY_GROWTH=1 -s WASM=1 -s ASYNCIFY=1
        -s DYNCALLS=1 -s ASSERTIONS=1
        -s TOTAL_STACK=33554432 -s TOTAL_MEMORY=134217728)

    get_target_property(resource_names resources NAMES)
    foreach(resource ${resource_names})
        list(APPEND LINK_FLAGS --preload-file ${resource})
    endforeach()

    if(CMAKE_BUILD_TYPE STREQUAL Debug)
        list(APPEND LINK_FLAGS
            --emrun --emit-symbol-map
            -s DEMANGLE_SUPPORT=1
            -s SAFE_HEAP=1)
    endif()

    target_sources(solvespace-gui PRIVATE
        ${CMAKE_SOURCE_DIR}/src/platform/guihtml.cpp)

    string(REPLACE ";" " " LINK_FLAGS "${LINK_FLAGS}")
    set_target_properties(solvespace-gui PROPERTIES
        LINK_FLAGS "${LINK_FLAGS}")
    set_source_files_properties(${CMAKE_SOURCE_DIR}/src/platform/guihtml.cpp PROPERTIES
        OBJECT_DEPENDS ${SHELL})

    add_custom_command(
        TARGET solvespace-gui POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_CURRENT_SOURCE_DIR}/platform/html/solvespaceui.css
                ${EXECUTABLE_OUTPUT_PATH}/solvespaceui.css
        COMMENT "Copying UI stylesheet"
        VERBATIM)
    add_custom_command(
        TARGET solvespace-gui POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_CURRENT_SOURCE_DIR}/platform/html/solvespaceui.js
                ${EXECUTABLE_OUTPUT_PATH}/solvespaceui.js
        COMMENT "Copying UI script solvespaceui.js"
        VERBATIM)
    add_custom_command(
        TARGET solvespace-gui POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_CURRENT_SOURCE_DIR}/platform/html/filemanagerui.js
                ${EXECUTABLE_OUTPUT_PATH}/filemanagerui.js
        COMMENT "Copying UI script filemanagerui.sj"
        VERBATIM)
else()
    target_sources(solvespace-gui PRIVATE
        ${CMAKE_SOURCE_DIR}/src/platform/guigtk.cpp)

    target_include_directories(solvespace-gui PRIVATE SYSTEM
        ${GTKMM_INCLUDE_DIRS}
        ${JSONC_INCLUDE_DIRS}
        ${FONTCONFIG_INCLUDE_DIRS})
    target_link_directories(solvespace-gui PRIVATE
        ${GTKMM_LIBRARY_DIRS}
        ${JSONC_LIBRARY_DIRS}
        ${FONTCONFIG_LIBRARY_DIRS})
    target_link_libraries(solvespace-gui PRIVATE
        ${GTKMM_LIBRARIES}
        ${JSONC_LIBRARIES}
        ${FONTCONFIG_LIBRARIES})
endif()

if(MSVC)
    set_target_properties(solvespace-gui PROPERTIES
        LINK_FLAGS "/MANIFEST:NO /SAFESEH:NO /INCREMENTAL:NO /OPT:REF")
endif()

# solvespace unix package

if(NOT (WIN32 OR APPLE OR EMSCRIPTEN))
    install(TARGETS solvespace-gui
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

if(APPLE)
    # copy debug symbols
    target_compile_options(solvespace-gui PRIVATE -g)
    if(ENABLE_LTO)
        set(lto_object ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/solvespace-gui-lto.o)
        set_property(TARGET solvespace-gui APPEND_STRING PROPERTY
        LINK_FLAGS " -Wl,-object_path_lto -Wl,${lto_object}")
    endif()
    add_custom_command(TARGET solvespace-gui POST_BUILD
        COMMAND xcrun dsymutil $<TARGET_FILE:solvespace-gui>
        COMMAND xcrun strip -Sl $<TARGET_FILE:solvespace-gui>
        COMMAND cp -r ${CMAKE_BINARY_DIR}/Resources $<TARGET_BUNDLE_CONTENT_DIR:solvespace-gui>
    )
    # copy libomp
    if(ENABLE_OPENMP)
        add_custom_command(TARGET solvespace-gui POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${OpenMP_CXX_LIBRARIES} $<TARGET_BUNDLE_CONTENT_DIR:solvespace-gui>/Resources/libomp.dylib
            COMMAND ${CMAKE_INSTALL_NAME_TOOL} -change `otool -L $<TARGET_FILE:solvespace-gui> | grep libomp | cut -d ' ' -f1 | xargs echo` "@rpath/libomp.dylib" $<TARGET_FILE:solvespace-gui>
        )
    endif()
endif()