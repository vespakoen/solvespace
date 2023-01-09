message(STATUS "Building core")

#===================#
# CORE DEPENDENCIES #
#===================#

add_library(slvs_deps INTERFACE)

target_include_directories(slvs_deps INTERFACE SYSTEM
    ${OPENGL_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIR}
    ${PNG_PNG_INCLUDE_DIR}
    ${FREETYPE_INCLUDE_DIRS}
    ${CAIRO_INCLUDE_DIRS}
    ${MIMALLOC_INCLUDE_DIR}
    ${EIGEN3_INCLUDE_DIRS})

target_link_libraries(slvs_deps INTERFACE
    dxfrw
    ${ZLIB_LIBRARY}
    ${PNG_LIBRARY}
    ${FREETYPE_LIBRARY}
    ${CAIRO_LIBRARIES}
    mimalloc-static)

if(Backtrace_FOUND)
    target_include_directories(slvs_deps INTERFACE SYSTEM
        ${Backtrace_INCLUDE_DIRS})
    target_link_libraries(slvs_deps INTERFACE
        ${Backtrace_LIBRARY})
endif()

if(SPACEWARE_FOUND)
    target_include_directories(slvs_deps INTERFACE SYSTEM
        ${SPACEWARE_INCLUDE_DIR})
    target_link_libraries(slvs_deps INTERFACE
        ${SPACEWARE_LIBRARIES})
endif()

if(ENABLE_OPENMP)
    target_link_libraries(slvs_deps INTERFACE slvs_openmp)
endif()

target_compile_options(slvs_deps
    INTERFACE ${COVERAGE_FLAGS})

if(APPLE)
    target_link_libraries(slvs_deps INTERFACE
        ${APPKIT_LIBRARY})
endif()

#==============#
# CORE LIBRARY #
#==============#

add_library(
    solvespace-core
STATIC
    ${CMAKE_SOURCE_DIR}/src/dsc.h
    ${CMAKE_SOURCE_DIR}/src/expr.h
    ${CMAKE_SOURCE_DIR}/src/polygon.h
    ${CMAKE_SOURCE_DIR}/src/sketch.h
    ${CMAKE_SOURCE_DIR}/src/solvespace.h
    ${CMAKE_SOURCE_DIR}/src/ui.h
    ${CMAKE_SOURCE_DIR}/src/platform/platform.h
    ${CMAKE_SOURCE_DIR}/src/render/render.h
    ${CMAKE_SOURCE_DIR}/src/render/gl3shader.h
    ${CMAKE_SOURCE_DIR}/src/srf/surface.h
    ${CMAKE_SOURCE_DIR}/src/bsp.cpp
    ${CMAKE_SOURCE_DIR}/src/clipboard.cpp
    ${CMAKE_SOURCE_DIR}/src/confscreen.cpp
    ${CMAKE_SOURCE_DIR}/src/constraint.cpp
    ${CMAKE_SOURCE_DIR}/src/constrainteq.cpp
    ${CMAKE_SOURCE_DIR}/src/describescreen.cpp
    ${CMAKE_SOURCE_DIR}/src/draw.cpp
    ${CMAKE_SOURCE_DIR}/src/drawconstraint.cpp
    ${CMAKE_SOURCE_DIR}/src/drawentity.cpp
    ${CMAKE_SOURCE_DIR}/src/entity.cpp
    ${CMAKE_SOURCE_DIR}/src/export.cpp
    ${CMAKE_SOURCE_DIR}/src/exportstep.cpp
    ${CMAKE_SOURCE_DIR}/src/exportvector.cpp
    ${CMAKE_SOURCE_DIR}/src/expr.cpp
    ${CMAKE_SOURCE_DIR}/src/file.cpp
    ${CMAKE_SOURCE_DIR}/src/generate.cpp
    ${CMAKE_SOURCE_DIR}/src/graphicswin.cpp
    ${CMAKE_SOURCE_DIR}/src/group.cpp
    ${CMAKE_SOURCE_DIR}/src/groupmesh.cpp
    ${CMAKE_SOURCE_DIR}/src/importdxf.cpp
    ${CMAKE_SOURCE_DIR}/src/importidf.cpp
    ${CMAKE_SOURCE_DIR}/src/importmesh.cpp
    ${CMAKE_SOURCE_DIR}/src/mesh.cpp
    ${CMAKE_SOURCE_DIR}/src/modify.cpp
    ${CMAKE_SOURCE_DIR}/src/mouse.cpp
    ${CMAKE_SOURCE_DIR}/src/polyline.cpp
    ${CMAKE_SOURCE_DIR}/src/polygon.cpp
    ${CMAKE_SOURCE_DIR}/src/resource.cpp
    ${CMAKE_SOURCE_DIR}/src/request.cpp
    ${CMAKE_SOURCE_DIR}/src/style.cpp
    ${CMAKE_SOURCE_DIR}/src/system.cpp
    ${CMAKE_SOURCE_DIR}/src/textscreens.cpp
    ${CMAKE_SOURCE_DIR}/src/textwin.cpp
    ${CMAKE_SOURCE_DIR}/src/toolbar.cpp
    ${CMAKE_SOURCE_DIR}/src/ttf.cpp
    ${CMAKE_SOURCE_DIR}/src/undoredo.cpp
    ${CMAKE_SOURCE_DIR}/src/util.cpp
    ${CMAKE_SOURCE_DIR}/src/view.cpp
    ${CMAKE_SOURCE_DIR}/src/platform/platform.cpp
    ${CMAKE_SOURCE_DIR}/src/platform/gui.cpp
    ${CMAKE_SOURCE_DIR}/src/render/render.cpp
    ${CMAKE_SOURCE_DIR}/src/render/render2d.cpp
    ${CMAKE_SOURCE_DIR}/src/srf/boolean.cpp
    ${CMAKE_SOURCE_DIR}/src/srf/curve.cpp
    ${CMAKE_SOURCE_DIR}/src/srf/merge.cpp
    ${CMAKE_SOURCE_DIR}/src/srf/ratpoly.cpp
    ${CMAKE_SOURCE_DIR}/src/srf/raycast.cpp
    ${CMAKE_SOURCE_DIR}/src/srf/shell.cpp
    ${CMAKE_SOURCE_DIR}/src/srf/surface.cpp
    ${CMAKE_SOURCE_DIR}/src/srf/surfinter.cpp
    ${CMAKE_SOURCE_DIR}/src/srf/triangulate.cpp
)

target_include_directories(
    solvespace-core
PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/src
)

target_link_libraries(solvespace-core PUBLIC slvs_deps)

#==============#
# TRANSLATIONS #
#==============#

set(translation_sources
    ${CMAKE_SOURCE_DIR}/src/platform/guiwin.cpp
    ${CMAKE_SOURCE_DIR}/src/platform/guigtk.cpp
    ${CMAKE_SOURCE_DIR}/src/platform/guimac.mm
    ${CMAKE_SOURCE_DIR}/src/platform/guihtml.cpp
    ${CMAKE_SOURCE_DIR}/src/solvespace.cpp)

if(HAVE_GETTEXT)
    get_target_property(solvespace_core_sources solvespace-core SOURCES)
    set(inputs
        ${solvespace_core_sources}
        ${translation_sources})

    set(templ_po   ${CMAKE_BINARY_DIR}/res/messages.po)
    set(output_pot ${CMAKE_SOURCE_DIR}/res/messages.pot)
    set(output_po  ${CMAKE_SOURCE_DIR}/res/locales/en_US.po)
    file(GLOB locale_pos ${CMAKE_SOURCE_DIR}/res/locales/*.po)

    string(REPLACE ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR}
        gen_output_pot ${output_pot}.gen)
    string(REPLACE ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR}
        gen_output_po ${output_po}.gen)
    foreach(locale_po ${locale_pos})
        string(REPLACE ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR}
            gen_locale_po ${locale_po}.gen)
        list(APPEND gen_locale_pos ${gen_locale_po})
    endforeach()

    add_custom_command(
        OUTPUT  ${gen_output_pot}
        COMMAND ${XGETTEXT}
                --language=C++
                --keyword --keyword=_ --keyword=N_ --keyword=C_:2,1c --keyword=CN_:2,1c
                --force-po --width=100 --sort-by-file
                --package-name=SolveSpace
                --package-version=${PROJECT_VERSION}
                "--copyright-holder=the PACKAGE authors"
                --msgid-bugs-address=whitequark@whitequark.org
                --from-code=utf-8 --output=${gen_output_pot} ${inputs}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${gen_output_pot} ${output_pot}
        DEPENDS ${inputs}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Extracting translations"
        VERBATIM)

    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/res/locales)

    # en_US is a bit special; we pre-fill the msgstrs from msgids, instead of (as would normally
    # happen) leaving them empty.
    add_custom_command(
        OUTPUT  ${gen_output_po}
        COMMAND ${MSGINIT}
                --locale=en_US --no-translator
                --output=${templ_po} --input=${gen_output_pot}
        COMMAND ${MSGMERGE}
                --force-po --no-fuzzy-matching
                --output=${gen_output_po} ${output_po} ${templ_po}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${gen_output_po} ${output_po}
        DEPENDS ${gen_output_pot}
        COMMENT "Updating en_US translations"
        VERBATIM)

    foreach(locale_po ${locale_pos})
        string(REPLACE ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR}
               gen_locale_po ${locale_po}.gen)

        get_filename_component(locale_name ${locale_po} NAME_WE)
        if(locale_name STREQUAL "en_US")
            continue()
        endif()

        add_custom_command(
            OUTPUT  ${gen_locale_po}
            COMMAND ${MSGMERGE}
                    --no-fuzzy-matching
                    --output=${gen_locale_po} ${locale_po} ${gen_output_pot}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${gen_locale_po} ${locale_po}
            DEPENDS ${gen_output_pot}
            COMMENT "Updating ${locale_name} translations"
            VERBATIM)
    endforeach()

    add_custom_target(translate_solvespace
        DEPENDS ${gen_output_pot} ${gen_output_po} ${gen_locale_pos})
endif()
