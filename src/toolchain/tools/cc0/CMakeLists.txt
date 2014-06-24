cmake_minimum_required(VERSION 2.8)

add_executable(cc0
    main.cpp
)

add_dependencies(cc0 core disa i0 langc flat)

target_link_libraries(cc0 core langc disa i0 flat boost_serialization)
target_link_libraries(cc0 ${BOEHMGC_LIBRARIES})


install(TARGETS cc0 DESTINATION bin)

