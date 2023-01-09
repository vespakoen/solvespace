cmake_minimum_required(VERSION 3.15...3.25)

project(CDemo LANGUAGES C)

find_package(SolveSpace REQUIRED)

add_executable(
  ${PROJECT_NAME}
  ${PROJECT_SOURCE_DIR}/CDemo.c
)

target_link_libraries(
  ${PROJECT_NAME}
  SolveSpace::slvs
)

if(WIN32)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND_EXPAND_LISTS
  )
endif()
