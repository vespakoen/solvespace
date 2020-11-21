set(CMAKE_CXX_STANDARD 11)

add_executable(slvslib
    main.cpp)

target_link_libraries(slvslib
    slvs)

set_target_properties(slvslib PROPERTIES
    LINK_FLAGS "-s TOTAL_MEMORY=134217728 --bind -std=c++11 -s ASSERTIONS=1 -s MODULARIZE=1")
