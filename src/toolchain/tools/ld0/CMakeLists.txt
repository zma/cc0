cmake_minimum_required(VERSION 2.8)

add_executable(ld0
    main.cpp
)

add_dependencies(ld0 core i0 disa flat langc)

target_link_libraries(ld0 core i0 disa flat langc boost_serialization)

install(TARGETS ld0 DESTINATION bin)

